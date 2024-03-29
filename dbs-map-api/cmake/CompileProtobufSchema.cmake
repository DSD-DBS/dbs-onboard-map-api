# SPDX-FileCopyrightText: Copyright DB Netz AG
# SPDX-License-Identifier: CC0-1.0

# Select HERE OLP .proto files.
# Skip Google .proto files.
file(GLOB_RECURSE PROTOBUF_FILELIST RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "*.proto")
list(FILTER PROTOBUF_FILELIST EXCLUDE REGEX "^google")

# Generate c++ classes for selected .proto files.
set(PROTOBUF_GENERATE_CPP_APPEND_PATH OFF)
PROTOBUF_GENERATE_CPP(PROTO_SRC PROTO_HEADER ${PROTOBUF_FILELIST})

include_directories(
    ${PROTOBUF_INCLUDE_DIRS}
    ${CMAKE_CURRENT_BINARY_DIR})

get_filename_component(PROTO_SCHEMA_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)

add_library(${PROTO_SCHEMA_NAME} ${PROTO_HEADER} ${PROTO_SRC})

install(TARGETS ${PROTO_SCHEMA_NAME} EXPORT dbs-map-apiTargets)
