cmake_minimum_required(VERSION 3.16)

project(Lommer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
include(CheckIncludeFile)
include(CheckIncludeFiles)

# Включаем автоматическую обработку MOC, UIC и RCC для Qt
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(MSGPACK_USE_BOOST OFF) #Disable use boost
FetchContent_Declare(
        tomlplusplus
        GIT_REPOSITORY https://github.com/marzer/tomlplusplus.git
        GIT_TAG         v3.4.0
)
FetchContent_MakeAvailable(tomlplusplus)

FetchContent_Declare(
        json
        URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
)
FetchContent_MakeAvailable(json)
add_compile_options(
    -fno-extended-identifiers
    -fstrict-aliasing
    -Wall
    -Wextra
    -Werror
    -Wno-comment
    -Wdouble-promotion
    -Wswitch-default
    -Wswitch-enum
    -Wuninitialized
    -Wstrict-aliasing
    -Wfloat-equal
    -Wshadow
    -Wplacement-new
    -Wcast-align
    -Wconversion
    -Wlogical-op
    -Wduplicated-cond
    -Wduplicated-branches
    -Wrestrict
    -Wnull-dereference
)

if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux" AND ${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fsanitize=undefined -fno-sanitize=vptr")
endif()

add_subdirectory(Core)

if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux" AND ${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fsanitize=undefined -fno-sanitize=vptr")
endif()

add_subdirectory(ClientDesktop)
add_subdirectory(Server)

