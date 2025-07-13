//
// Created by backo on 02/07/25.
//

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wc++23-extensions"
#ifndef DPP_STRUCTURES_SERVICE_WEB_H
#define DPP_STRUCTURES_SERVICE_WEB_H

#include <type_traits>
#include <utility>
#include "feature.h"
#include "internal/string_literal.h"
#include "internal/meta.h"
#include "utilities/singleton.h"
#include "utilities/text.h"

#include <cstdarg>


namespace dpp_structures::http {


    enum query_key_type {
        REQUIRED,
        OPTIONAL
    };

    /*
     * The base class query is used for passing query values. The derived classes optional
     * and required are used for specifying which query parameters a method accepts.
     */
    template<internal::string_literal TKey>
    struct query {

        static constexpr internal::string_literal Key = TKey;

        query(bool b) : value(b ? "true" : "false") {}

        query(std::string value) : value(std::move(value)) {};

        query() = default;

        std::string value;

    };

    template<internal::string_literal Key>
    struct required : query<Key> {
        using query<Key>::query;

        static constexpr query_key_type type() {
            return REQUIRED;
        }
    };

    template<internal::string_literal Key>
    struct optional : query<Key> {
        using query<Key>::query;

        static constexpr query_key_type type() {
            return OPTIONAL;
        }
    };


    /**
     * Check if T is a template instantiation of an optional or required query key.
     */
    template<typename T>
    concept QueryKeyDescr = std::is_same_v<required<T::Key>, T> || std::is_same_v<optional<T::Key>, T>;

    template<typename T>
    concept QueryKeyValue = std::is_same_v<T, query<T::Key>>;


    template<size_t N>
    class path_params {
    public:
        std::array<std::string, N> array;

        template<typename... T>
        path_params(T&& ... values) : array({values...}) {

        }
    };

    template<typename... T>
    path_params(T&& ... t) -> path_params<sizeof...(T)>;


    template<dpp::http_method HttpMethod, internal::string_literal Name, internal::string_literal LiteralURL, QueryKeyDescr... QueryKeys>
    class method
            : public singleton<method<HttpMethod, Name, LiteralURL, QueryKeys...>, SingletonInstanceType::REFERENCE> {
    public:
        static constexpr size_t query_parameters_num = sizeof...(QueryKeys);

    private:
        /**
         *
         * @return A pair indicating the count of parameters and whether LiteralURL is well-formed: {count, well_formed}
         */
        static consteval std::pair<size_t, bool> count_check_path_parameters() {
            size_t num = 0;
            bool open = false;
            for (size_t i = 0; i < LiteralURL.size(); ++i) {
                char c = LiteralURL[i];

                if ((open && c == '{') || (!open && c == '}')) return {0, false};

                if (c == '{') open = true;
                else if (c == '}') {
                    open = false;
                    ++num;
                }
            }
            return {num, !open};

        }

        static consteval size_t count_path_parameters() {
            constexpr auto p = count_check_path_parameters();
            static_assert(p.second, "unclosed brackets");
            return p.first;
        }

        struct check_query_parameters_result {
            std::optional<std::string_view> duplicate = std::nullopt;
            int missing_required = 0;
            std::optional<std::string_view> unknown_key = std::nullopt;
        };


        template<QueryKeyValue... TargetQueryKeys>
        static consteval check_query_parameters_result check_query_parameters() {
            constexpr std::array<std::pair<std::string_view, query_key_type>, sizeof...(QueryKeys)> allQueryKeys = {
                    std::pair{std::string_view(QueryKeys::Key), QueryKeys::type()}...
            };
            constexpr std::array<std::string_view, sizeof...(TargetQueryKeys)> targetKeys = {
                    std::string_view(TargetQueryKeys::Key)...
            };

            // Check for duplicates
            for (std::size_t i = 0; i < targetKeys.size(); ++i) {
                for (std::size_t j = i + 1; j < targetKeys.size(); ++j) {
                    if (targetKeys[i] == targetKeys[j]) return {.duplicate = targetKeys[i]}; // duplicate
                }
            }

            int requiredKeysNum = 0;

            for (const auto& [key, key_type] : allQueryKeys) {
                if (key_type == REQUIRED) {
                    ++requiredKeysNum;
                }
            }


            for (const auto& key : targetKeys) {
                bool found = false;
                for (auto& [k, type] : allQueryKeys) {
                    if (key == k) {
                        found = true;
                        if (type == REQUIRED) --requiredKeysNum;
                        break;
                    }
                }
                if (!found) return {.unknown_key = key};
            }

            return {.missing_required = requiredKeysNum};
        }

    public:
        static constexpr size_t path_parameters_num = count_path_parameters();
        static constexpr std::string_view url = LiteralURL.data;
        static constexpr std::string_view name = Name.data;
        static constexpr dpp::http_method httpMethod = HttpMethod;

        template<QueryKeyValue... MyQueryKeys>
        std::string operator()(MyQueryKeys... queryKeys) {
            return this->operator()(path_params{}, queryKeys...);
        }

        template<std::size_t PathParamsLength, QueryKeyValue... MyQueryKeys>
        std::string operator()(const path_params<PathParamsLength>& path_params, MyQueryKeys... queryKeys) {


            static_assert(PathParamsLength == path_parameters_num);

            constexpr auto check_query_result = check_query_parameters<MyQueryKeys...>();
            static_assert(check_query_result.unknown_key == std::nullopt, "Found unknown query key ");
            static_assert(check_query_result.duplicate == std::nullopt, "Found duplicate query key");
            static_assert(check_query_result.missing_required == 0, "Missing required query keys");

            std::string new_url;

            // Substitute path parameters
            auto path_params_it = path_params.array.cbegin();
            for (std::string_view::const_iterator url_it = url.cbegin(); url_it != url.cend(); ++url_it) {
                if (*url_it == '{') {
                    new_url.append(*path_params_it);
                    ++path_params_it;
                    do {
                        ++url_it;
                    } while (*url_it != '}');
                } else new_url.push_back(*url_it);

            }

            std::map<std::string, std::string> query_params = {{queryKeys.Key.data, queryKeys.value}...};

            // Add query parameters
            if (!query_params.empty()) {
                new_url.push_back('?');
                int i = 0;
                for (const auto& [key, value] : query_params) {
                    ++i;
                    new_url.append(key);

                    if (!value.empty())
                        new_url.append("=" + value);

                    if (i != query_params.size()) new_url.push_back('&');
                }
            }

            return new_url;
        }
    };


    template<typename T>
    struct is_method : std::false_type {
    };

    template<dpp::http_method HttpMethod, internal::string_literal Name, internal::string_literal LiteralURL, QueryKeyDescr... QueryKeys>
    struct is_method<method<HttpMethod, Name, LiteralURL, QueryKeys...>> : std::true_type {
    };

    template<typename T>
    concept MethodType = is_method<T>::value;


    template<MethodType Method>
    struct resolved_method {

        Method& method;

        std::string url;

        template<typename... Args>
        resolved_method(Args... args) : method(Method::get_instance()), url(this->method(args...)) {}
    };

    template<MethodType Method>
    auto make_resolved_method(path_params<Method::path_parameters_num> path_params, auto... queryKeys) {
        return resolved_method<Method>{std::move(path_params), std::move(queryKeys)...};
    }

    template<internal::string_literal Target, typename First, typename... Rest>
    consteval auto lookup_method() {
        if constexpr (First::name == Target) {
            return [] { return *First::get_instance(); };
        } else if constexpr (sizeof...(Rest) > 0) {
            return lookup_method<Target, Rest...>();
        } else {
            static_assert(false, "No method found");
        }
    }

    template<internal::string_literal Target, typename First, typename... Rest>
    struct lookup_method_type {
        using type = std::remove_reference_t<decltype(([] -> decltype(auto) {
            if constexpr (First::name == Target) {
                return internal::declval<First>();
            } else if constexpr (sizeof...(Rest) > 0) {
                return internal::declval<typename lookup_method_type<Target, Rest...>::type>();
            } else {
                static_assert(false);
            }
        })())>;

    };


    template<internal::string_literal BaseUrl, MethodType... Methods>
    class service_web : feature {
    private:
        template<dpp::http_method HttpMethod>
        requires (HttpMethod != dpp::m_post)
        dpp::task<dpp::http_request_completion_t>
        request(const std::string& url, const std::string& contentType = "application/json") {
            co_await this->pre_request(url);
            co_return co_await this->bot->co_request(url, HttpMethod, "", contentType,
                                                     this->get_default_headers());
        };


        dpp::task<dpp::http_request_completion_t> request_post(const std::string& url, const std::string& body,
                                                               const std::string& contentType = "application/json") {
            co_await this->pre_request(url);
            co_return co_await this->bot->co_request(url, dpp::m_post, body, contentType,
                                                     this->get_default_headers());

        }

    protected:
        using feature::bot;

        virtual dpp::task<void> pre_request(const std::string& url) {
            co_return;
        }

    public:
        template<internal::string_literal Name>
        using find_method = typename lookup_method_type<Name, Methods...>::type;


        template<internal::string_literal Name>
        static auto method() {
            return lookup_method<Name, Methods...>();
        }

        /*
         * Shortcut to call operator() of the method.
         */
        template<internal::string_literal Name, typename... Args>
        static std::string method(Args... args) {
            return (find_method<Name>::get_instance());
        };

        std::string base_url = BaseUrl.data;

        explicit service_web() = default;

        /**
         * Authentication headers go here
         * @return
         */
        virtual std::multimap<std::string, std::string> get_default_headers() {
            return {};
        }

        template<MethodType Method>
        requires (Method::httpMethod == dpp::http_method::m_post)
        dpp::task<dpp::http_request_completion_t> post(resolved_method<Method> resolved_method, nlohmann::json body,
                                                       const std::string& contentType = "application/json") {

            co_return co_await this->request_post(join_paths(this->base_url, resolved_method.url), body, contentType);
        };

        template<MethodType Method>
        requires (Method::httpMethod == dpp::http_method::m_get)
        dpp::task<dpp::http_request_completion_t>
        get(const resolved_method<Method>& resolved_method, const std::string& contentType = "application/json") {

            co_return co_await this->request<dpp::m_get>(join_paths(this->base_url, resolved_method.url),
                                                         contentType);
        };


    };


}
#endif //DPP_STRUCTURES_SERVICE_WEB_H

#pragma clang diagnostic pop