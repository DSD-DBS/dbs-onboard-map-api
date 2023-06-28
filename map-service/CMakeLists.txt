# SPDX-FileCopyrightText: Copyright DB Netz AG
# SPDX-License-Identifier: CC0-1.0

set(MAP_SERVICE_BOOST_VERSION 1.65.0)
find_package(Boost ${MAP_SERVICE_BOOST_VERSION} REQUIRED)

# find_package(Boost ${MAP_SERVICE_BOOST_VERSION} COMPONENTS system filesystem REQUIRED)
if(WIN32)
    # find_package(protobuf CONFIG REQUIRED)
    # TODO: vcpkg specific pathes are hardcoded.
    # Consider to get rid of hardcodded pathes
    set(Protobuf_INCLUDE_DIRS C:/vcpkg/packages/protobuf_x64-windows/include)
    set(Protobuf_LIBRARIES C:/vcpkg/packages/protobuf_x64-windows/lib)
endif()

find_package(Protobuf REQUIRED)

if(PROTOBUF_FOUND)
    message("protobuf found")
endif()

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/cmake")

file(GLOB PROTO_SCHEMA_SOURCE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/proto/*/*")

list(APPEND PROTO_SCHEMA_LIBS "")

foreach(PROTO_SCHEMA_SOURCE_PATH ${PROTO_SCHEMA_SOURCE_DIRS})
    get_filename_component(PROTO_SCHEMA_NAME ${PROTO_SCHEMA_SOURCE_PATH} NAME)
    list(APPEND PROTO_SCHEMA_LIBS ${PROTO_SCHEMA_NAME})

    add_subdirectory(${PROTO_SCHEMA_SOURCE_PATH})
endforeach()

file(GLOB_RECURSE INC "include/*.h*")
file(GLOB_RECURSE SRC "src/*.*")

add_library(${PROJECT_NAME}
    SHARED
    ${SRC}
    ${INC}
    ${CMAKE_CURRENT_SOURCE_DIR}/../externals/smasher/src/MurmurHash3.cpp)
add_library(map-service::map-service ALIAS map-service)

if(ENABLE_TESTS)
    message("Code coverage is enabled")
    target_compile_options(${PROJECT_NAME} PUBLIC -O0 -g --coverage)
    target_link_options(${PROJECT_NAME} PUBLIC --coverage)
endif()

file(GLOB PROTO_SCHEMA_BINARY_DIRS "${CMAKE_CURRENT_BINARY_DIR}/proto/*/*")
target_include_directories(${PROJECT_NAME}
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
    ${PROTOBUF_INCLUDE_DIRS}
    ${PROTO_SCHEMA_BINARY_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/../externals) # TODO: it is here for Smasher only

target_link_libraries(${PROJECT_NAME}
    PUBLIC
    olp-cpp-sdk-core
    curlpp
    ${PROTOBUF_LIBRARIES}
    ${PROTO_SCHEMA_LIBS}
)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND ENABLE_TESTS)
    enable_testing()
    include(GoogleTest)
    add_subdirectory(tests)
endif()

set(PERSISTENT_STORAGE_ROOT_DIR $ENV{PERSISTENT_STORAGE_ROOT_DIR})
message("PERSISTENT_STORAGE_ROOT_DIR: " ${PERSISTENT_STORAGE_ROOT_DIR})

IF(NOT ${YOCTO_BUILD})
    IF(ENABLE_TESTS)
        file(COPY
            ${CMAKE_CURRENT_SOURCE_DIR}/hdmap
            DESTINATION
            ${CMAKE_BINARY_DIR})
    ELSE()
        file(COPY
            ${CMAKE_CURRENT_SOURCE_DIR}/hdmap
            DESTINATION
            ${PERSISTENT_STORAGE_ROOT_DIR})
    ENDIF()
ENDIF()

set(MAP_SERVICE_INSTALL_CMAKEDIR "${CMAKE_INSTALL_LIBDIR}/cmake/map-service")
install(FILES "${PROJECT_SOURCE_DIR}/map-service/cmake/configs/map-serviceConfig.cmake" DESTINATION ${MAP_SERVICE_INSTALL_CMAKEDIR})
install(
    DIRECTORY "${PROJECT_SOURCE_DIR}/map-service/include/"
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    FILES_MATCHING
    PATTERN "*.h"
)
install(TARGETS map-service EXPORT map-serviceTargets INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(
    EXPORT map-serviceTargets
    DESTINATION ${MAP_SERVICE_INSTALL_CMAKEDIR}
    NAMESPACE map-service::
)

# Workaround because olp installs their external dependencies in the build dir and uses the installed version. Instead
# they should use the builded version and install the dependencies where olp is installed as well And also specify their
# dependencies in their config file (see ./cmake/configs/map-ServiceConfig.cmake for the workaround there)
install(DIRECTORY "${PROJECT_BINARY_DIR}/externals/here-olp-sdk/install/" DESTINATION ${CMAKE_INSTALL_PREFIX})