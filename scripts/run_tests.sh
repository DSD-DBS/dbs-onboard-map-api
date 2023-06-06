#!/bin/sh

./build.sh -DCMAKE_BUILD_TYPE=Debug && ./build/map-service/tests/GoogleTestRunner "$@"