project(telegram_bot)

cmake_minimum_required(VERSION 3.8)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fsanitize=address,undefined -std=c++17 -O2 -Wall -Werror -Wsign-compare")

find_package(Poco QUIET COMPONENTS Foundation Net JSON)

if (NOT Poco_FOUND)
    message(STATUS "Seems like POCO is not installed on your machine.")
endif()

add_subdirectory(client)
add_subdirectory(bot)
add_subdirectory(bot_executable)