#!/bin/sh

# SPDX-FileCopyrightText: Copyright DB Netz AG
# SPDX-License-Identifier: CC0-1.0

./build.sh -DCMAKE_BUILD_TYPE=Debug && ./build/map-service/tests/GoogleTestRunner "$@"