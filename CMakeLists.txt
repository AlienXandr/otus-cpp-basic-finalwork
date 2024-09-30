cmake_minimum_required(VERSION 3.25)

project(emulator_server LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CXX_EXTENSIONS NO)

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_compile_options(
            -g
            -Wall
            -Wextra
            -Werror
            -pedantic)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    add_compile_options(
            /EHsc
            /W4
            /WX)
endif ()

add_executable(${PROJECT_NAME}
        main.cpp

        src/device_emulator/device_emulator.hpp
        src/device_emulator/device_emulator.cpp
        src/device_emulator/interfaces/IJsonMsg.h
        src/device_emulator/interfaces/IJsonMsg.cpp
        src/device_emulator/interfaces/IBinaryMsg.h
        src/device_emulator/helpers/getRandomData.h
        src/device_emulator/helpers/getMetadata.h

        src/server/server.hpp
        src/server/server.cpp
        src/server/clientList.h

        src/research_tests/json_test.hpp
)

find_package(Boost REQUIRED)
find_package(nlohmann_json REQUIRED)
target_link_libraries(${PROJECT_NAME}
        Boost::headers
        nlohmann_json::nlohmann_json
)
