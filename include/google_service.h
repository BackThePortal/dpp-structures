//
// Created by backo on 10/07/25.
//

#ifndef DPP_STRUCTURES_GOOGLE_SERVICE_H
#define DPP_STRUCTURES_GOOGLE_SERVICE_H

#include "internal/string_literal.h"
#include "google_service_account_manager.h"
#include "service_web.h"

namespace dpp_structures::services {

    template<internal::string_literal BaseUrl, http::MethodType... Methods>
    class google_service : public http::service_web<BaseUrl, Methods...> {
	protected:
		using http::service_web<BaseUrl, Methods...>::query;
    private:
        dpp::task<void> pre_request(const std::string& url) override {
            google_service_account_manager::request_token_result result = co_await google_service_account_manager::get_instance()->get_or_refresh_token();

            co_return;
        }

        std::multimap<std::string, std::string> get_default_headers() override {
            assert(google_service_account_manager::get_instance()->current_token != nullptr);

            return std::multimap<std::string, std::string>{
                    {"Authorization", "Bearer " + google_service_account_manager::get_instance()->current_token->token}
            };
        }
    };
}

#endif //DPP_STRUCTURES_GOOGLE_SERVICE_H
