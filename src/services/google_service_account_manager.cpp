//
// Created by backo on 03/07/25.
//
#include "google_service_account_manager.h"

#include <utility>

using namespace dpp_structures::services;

const std::string google_service_account_manager::grant_type = "urn:ietf:params:oauth:grant-type:jwt-bearer";
const std::time_t google_service_account_manager::token_duration = 3595;

google_service_account_manager::credentials_file_class::keys_array google_service_account_manager::credentials_keys_array{
        {
                {"private_key_id", internal_file_key_info::value_type::STRING},
                {"private_key", internal_file_key_info::value_type::STRING},
                {"client_email", internal_file_key_info::value_type::STRING}
        }};

google_service_account_manager::google_service_account_manager(const std::string& credentials_path,
                                                               const std::vector<std::string>& scopes)
        : credentials(credentials_path, google_service_account_manager::credentials_keys_array), scopes(scopes) {}

std::string google_service_account_manager::get_private_key() {
    std::cout << "get_private_key\n";

    const std::optional<std::string>& private_key = this->credentials.get(PRIVATE_KEY);
    assert(private_key.has_value());
    return private_key.value();
}

std::string
google_service_account_manager::create_header(const std::string& algorithm, const std::string& format) {
    std::cout << "create_header\n";

    return nlohmann::json{{"alg", algorithm},
                          {"typ", format},
                          {"kid", this->credentials(credentials_keys::PRIVATE_KEY_ID)}}.dump();
}

std::string google_service_account_manager::create_claim(const std::string& target) {
    std::cout << "create_claim\n";
    long long issue = std::time(nullptr);
    long long expiration = issue + google_service_account_manager::token_duration;


    auto email_address = this->credentials.get(EMAIL_ADDRESS).operator std::string();
    std::cout << "email_address: " << email_address << '\n';

    return nlohmann::json{{"iss",   email_address},
                          {"sub",   email_address},
                          {"scope", join(this->scopes, " ")},
                          {"aud", target},
                          {"exp",   expiration},
                          {"iat",   issue}}.dump();

}

std::string google_service_account_manager::create_jwt() {
    std::cout << "create_jwt\n";

    std::string signature_base =
            base64_encode<base64_url_alphabet>(this->create_header()) + "." +
            base64_encode<base64_url_alphabet>(
                    this->create_claim());
    return signature_base + "." + base64_encode<base64_url_alphabet>(*this->generate_signature(signature_base));

}

dpp::task<google_service_account_manager::request_token_result>
google_service_account_manager::request_token() {
    std::cout << "request_token\n";

    // TODO: POST body seems to be formatted in the same way as query params. Might need to put this inside service_web
    const std::string& body =
            "grant_type=" + url_encode(google_service_account_manager::grant_type) + "&assertion=" +
            this->create_jwt();
    std::cout << "- BODY -\n" << body << '\n';

    auto result = co_await this->post(
            http::resolved_method<
                    typename google_oauth2_service::find_method<"token">
            >(), body, "application/x-www-form-urlencoded");

    nlohmann::json result_json;
    std::string exception_info;

    try { result_json = nlohmann::json::parse(result.body); }
    catch (nlohmann::json::exception& exception) {
        exception_info = exception.what();
    }

    bot->log(dpp::loglevel::ll_info, "token request body:\n" + result.body);

    if (!exception_info.empty()) {
        co_return {request_token_error{exception_info, result.body}, result.status};
    } else if (!result_json.contains("access_token")) {
        co_return {request_token_error{std::nullopt, result.body}, result.status};
    } else
        delete this->current_token;
        co_return {
                this->current_token = (new access_token{result_json["access_token"],
                                                        std::time(nullptr) + token_duration - 2}),
                result.status
        };
}

dpp::task<google_service_account_manager::request_token_result> google_service_account_manager::get_or_refresh_token() {
    if (this->current_token != nullptr && !this->current_token->has_expired())
        co_return {
                this->current_token, -1};
    else co_return co_await this->request_token();
}
