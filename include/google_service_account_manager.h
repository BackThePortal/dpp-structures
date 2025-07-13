//
// Created by backo on 03/07/25.
//

#ifndef DPP_STRUCTURES_GOOGLE_SERVICE_ACCOUNT_MANAGER_H
#define DPP_STRUCTURES_GOOGLE_SERVICE_ACCOUNT_MANAGER_H

#include <ctime>
#include <openssl/rsa.h>
#include "service_web.h"
#include "utilities/encode.h"
#include "utilities/rsa.h"
#include "file.h"
#include "internal_json_file.h"
#include "utilities/singleton.h"

namespace dpp_structures::services {

    using google_oauth2_service = http::service_web<"https://oauth2.googleapis.com/", http::method<dpp::m_post, "token", "token">>;

    class google_service_account_manager
            : google_oauth2_service, public dpp_structures::singleton<google_service_account_manager> {
    public:
        enum credentials_keys : std::size_t {
            PRIVATE_KEY_ID,
            PRIVATE_KEY,
            EMAIL_ADDRESS,

            KEYS_LENGTH
        };

        using credentials_file_class = internal_json_file<i_json_file, credentials_keys>;

        static credentials_file_class::keys_array credentials_keys_array;

    private:

        internal_json_file<i_json_file, credentials_keys> credentials;

        std::string get_private_key();

        inline auto generate_signature(const std::string& base) {
            return sign_sha256_rsa(base, this->get_private_key());
        }


        std::string create_header(const std::string& algorithm = "RS256",
                                  const std::string& format = "JWT");

        std::string create_claim(const std::string& target = "https://oauth2.googleapis.com/token");

        std::string create_jwt();


        google_service_account_manager(const std::string& credentials_path,
                                       const std::vector<std::string>& scopes);

        FRIEND_SINGLETON(google_service_account_manager)

    public:
        struct access_token {
            std::string token;
            std::time_t expiration;

            [[nodiscard]] inline bool has_expired() const {
                return std::time(nullptr) > this->expiration;
            }
        };

        static const std::string grant_type;
        static const std::time_t token_duration;

        std::vector<std::string> scopes;

        access_token* current_token = nullptr;


        struct request_token_error {
            std::optional<std::string> json_parse_error;
            std::string output;
        };

        struct request_token_result {
            std::variant<access_token*, request_token_error> result;
            int http_code;

            [[nodiscard]] inline bool has_token() const { return std::holds_alternative<access_token*>(this->result); }

            [[nodiscard]] inline bool is_error() const {
                return std::holds_alternative<request_token_error>(this->result);
            }

            inline access_token& get_token() {
                return *std::get<access_token*>(this->result);
            }

            inline request_token_error& get_error() {
                return std::get<request_token_error>(this->result);
            }
        };

        dpp::task<request_token_result> request_token();

        dpp::task<request_token_result> get_or_refresh_token();

    };

}

#endif //DPP_STRUCTURES_GOOGLE_SERVICE_ACCOUNT_MANAGER_H
