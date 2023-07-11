#!/bin/sh
# SPDX-FileCopyrightText: Copyright DB Netz AG
# SPDX-License-Identifier: CC0-1.0

DBS_ONBOARD_MAP_API_DIR=`pwd`
COVERAGE_DIR=$DBS_ONBOARD_MAP_API_DIR/build/coverage
EXCLUDE='.*tests/|.*externals/|.*build/|.*download/'

rm -rf $COVERAGE_DIR && mkdir -p $COVERAGE_DIR
# gcovr -r $DBS_ONBOARD_MAP_API_DIR --exclude $EXCLUDE --html --html-details -o $COVERAGE_DIR/coverage.html
gcovr -r $DBS_ONBOARD_MAP_API_DIR --exclude $EXCLUDE --xml $COVERAGE_DIR/coverage.xml
