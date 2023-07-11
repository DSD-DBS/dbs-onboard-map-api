#!/bin/sh
# SPDX-FileCopyrightText: Copyright DB Netz AG
# SPDX-License-Identifier: CC0-1.0

BUILD_DIR=`pwd`/build
# INSTALL_DIR=`pwd`/install

# rm -rf $BUILD_DIR
mkdir $BUILD_DIR
cd $BUILD_DIR
pwd

# Build
cmake "$@" ..
cmake --build . -j 10

# Install
# cmake --install $BUILD_DIR --prefix $INSTALL_DIR
