#!/bin/sh
# SPDX-FileCopyrightText: Copyright DB Netz AG
# SPDX-License-Identifier: CC0-1.0

BUILD_DIR="/Users/volodymyrbezkorovain/projects/GitHub/dbs-onboard-map-api/arch/build"
STRUCTURIZR_DIR="/Users/volodymyrbezkorovain/projects/GitHub/dbs-onboard-map-api/arch/build/structurizr-output"

rm -rf $BUILD_DIR && mkdir $BUILD_DIR && cd $BUILD_DIR && cmake .. && cmake --build . --target structurizr
cd  $STRUCTURIZR_DIR
for file in *.dot; do
    dot -Tpng "$file" -o "${file%.dot}.png"
done

