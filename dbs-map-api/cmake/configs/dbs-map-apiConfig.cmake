# SPDX-FileCopyrightText: Copyright DB Netz AG
# SPDX-License-Identifier: CC0-1.0

include(CMakeFindDependencyMacro)
find_dependency(olp-cpp-sdk-core)
find_dependency(leveldb) # Dependency of olp
find_dependency(Threads) # Dependency of leveldb
find_dependency(curlpp)

include(${CMAKE_CURRENT_LIST_DIR}/dbs-map-apiTargets.cmake)
