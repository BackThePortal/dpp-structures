cmake_minimum_required(VERSION 3.31)
project(dpp_structures)

set(CMAKE_CXX_STANDARD 20)

add_definitions(-DDPP_CORO)

add_library(dpp_structures STATIC
        src/bases/feature.cpp
        src/bases/listener.cpp
        src/bases/service.cpp

        src/commands/command.cpp
        src/commands/command_factories.cpp

        src/files/file.cpp
        src/files/internal_json_file.cpp

        src/utilities/dynamic_value.cpp
        src/utilities/singleton.cpp

        src/listeners/on_ready.cpp

        src/message_sender.cpp
        src/utilities/text.cpp
)

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
