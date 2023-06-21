# SPDX-FileCopyrightText: Copyright DB Netz AG
# SPDX-License-Identifier: CC0-1.0

# include(GoogleTest)
set(EXEC_NAME GoogleTestRunner)

file(GLOB_RECURSE TEST_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_executable(${EXEC_NAME} ${TEST_FILES})

target_link_libraries(${EXEC_NAME}
    gtest
    gmock
    map-service)