
# SPDX-FileCopyrightText: Copyright DB Netz AG
# SPDX-License-Identifier: CC0-1.0

cmake_minimum_required(VERSION 3.16)

# Build targets
project(map-service VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-Wno-deprecated-declarations")
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(RAPIDJSON_HAS_CXX11_RANGE_FOR ON)

option(ENABLE_TESTS "Enable Testing of map-service" ON)

# Add cmake dir to search list
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

option(YOCTO_BUILD "Is Yocto Build Environment" OFF)
IF( NOT ${YOCTO_BUILD})
    # only import when not using YOCTO
    if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND ENABLE_TESTS)
        add_subdirectory(externals/googletest)
    endif()

    # Explicitly allow overriding cache variables with local variables. This is standard behaviour in cmake 3.13 and above, but olp-sdk still uses 3.9
    set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)
    set(OLP_SDK_ENABLE_TESTING OFF)
    add_subdirectory(externals/here-olp-sdk)

    add_subdirectory(externals/curlpp)
    # add_subdirectory(externals/rapidjson)
ENDIF()

add_subdirectory(map-service)
add_subdirectory(docs)