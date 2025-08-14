cmake_minimum_required(VERSION 3.31)
project(dpp_structures)

set(CMAKE_CXX_STANDARD 23)


add_library(dpp_structures STATIC
        src/bases/feature.cpp
        src/bases/listener.cpp
        src/bases/service_cli.cpp
        src/bases/service_web.cpp

        src/commands/command.cpp
        src/commands/command_factories.cpp

        src/files/file.cpp
        src/files/internal_json_file.cpp

        src/utilities/dynamic_value.cpp
        src/utilities/singleton.cpp
        src/utilities/text.cpp
        src/utilities/encode.cpp
        src/utilities/rsa.cpp

        src/internal/exec.cpp

        src/listeners/on_ready.cpp

        src/services/service_web.cpp
        src/services/google_service_account_manager.cpp
        src/services/google_service.cpp

        src/message_sender.cpp
        src/command_router.cpp
        src/utilities/cases.cpp

        src/json/std_serializers.cpp
        src/json/macro_serializers.cpp
        src/json/polymorphic_serializer.cpp
        src/json/variant.cpp
        include/utilities/bitmask.h
        src/add_mentions.cpp
        src/feature_adapter.cpp

)

target_compile_definitions(dpp_structures PUBLIC DPP_CORO)

include_directories(
        include/
)

install(TARGETS dpp_structures
        DESTINATION lib
)

install(
        DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
        DESTINATION include/dpp-structures
        FILES_MATCHING
            PATTERN "*.h"
            PATTERN "version.h"
)
