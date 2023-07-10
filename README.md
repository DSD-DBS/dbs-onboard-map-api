<!--
 ~ SPDX-FileCopyrightText: Copyright DB Netz AG
 ~ SPDX-License-Identifier: CC0-1.0
 -->
[![codecov](https://codecov.io/github/DSD-DBS/dbs-onboard-map-api/branch/main/graph/badge.svg?token=fefCH21TRD)](https://codecov.io/github/DSD-DBS/dbs-onboard-map-api)
[![dbs-onboard-map-api CI](https://github.com/DSD-DBS/dbs-onboard-map-api/actions/workflows/ci.yml/badge.svg)](https://github.com/DSD-DBS/dbs-onboard-map-api/actions/workflows/ci.yml)

# DBS Onboard C++ Map API
DBS Onboard Map API is a robust C++ API engineered to enable seamless interaction with onboard digital map. It provides a framework for accessing map data stored directly on the device, bypassing the need for constant internet connectivity. With its onboard-centric design, it ensures efficient and reliable map data handling even in the most internet-limited environments.

In addition to its onboard-centric design, it provides the capacity to update maps with the latest data fetched from DB cloud.

## Features
* C++ API: It ensures real-time access to onboard map data: by area and by layers. This means you can selectively retrieve the portions of the map you're interested in, or focus on specific layers of information, offering increased flexibility in managing and utilizing map data.

* Over-the-Air Incremental Updates: One of the distinguishing features of MapService is its ability to perform Over-the-Air (OTA) incremental updates. This means that it can fetch and integrate just the changed parts of the map from the cloud, bypassing the need to download the entire map again. Map is hosted in DB intranet and users need to have access to DB intranet to update the map.

## Licenses
This project is compliant with the <a href="https://git.fsfe.org/reuse/docs/src/commit/d173a27231a36e1a2a3af07421f5e557ae0fec46/spec.md" target="_blank">REUSE Specification Version 3.0</a>.

Copyright DB Netz AG, licensed under Apache 2.0 (see full text in <a href="https://github.com/DSD-DBS/dbs-onboard-map-api/blob/main/LICENSE" target="_blank"> Apache 2.0</a>)

Dot-files, cmake and yaml filse are licensed under CC0-1.0 (see full text in
 <a href="https://github.com/DSD-DBS/dbs-onboard-map-api/blob/main/LICENSES/CC0-1.0.txt" target="_blank">CC0-1.0</a>)

 Map files are licensed under CC-BY-SA-3.0-DE license (see full text in  <a href="https://github.com/DSD-DBS/dbs-onboard-map-api/blob/main/LICENSES/CC-BY-SA-3.0-DE.txt" target="_blank">CC-BY-SA-3.0-DE</a>)

## Build

### Prerequisites
* A modern C++ compiler (C++17 or later is recommended).
* CMake (3.16 or later).
* Google Protobuf
* Boost 1.65.0 or higher
* OpenSSL

### Dependencies
* Google Test & Mock
* Libcurl
* RapidJSON
* SMHasher

### Linux dependencies
To install the dependencies on Linux, run the following command:
```bash
sudo apt-get update && sudo apt-get --yes install cmake libgmock-dev libgtest-dev rapidjson-dev libcurl4-openssl-dev protobuf-compiler libprotobuf-dev libboost-all-dev -y --no-install-recommends --fix-missing
```

### Compile
Clone sources from git repo and update submodules:
```bash
git submodule update --init --recursive
```

Run build script from the root folder.
```bash
./build.sh -DCMAKE_BUILD_TYPE=Debug
```

### Test
```bash
./build/dbs-map-api/tests/GoogleTestRunner
```
## Supported platforms
| Platform                   | Minimum requirement     |
| :------------------------- | :---------------------- |
| Ubuntu Linux               | GCC or Clang            |
| Embedded Linux (32 bit)    | GCC                     |
| macOS                      | Apple Clang or GCC      |

## C++ API examples
Request certain area

```cpp
#include <dbs-map-api/MapService.h>
#include <dbs-map-api/MapServiceConfig.h>

using namespace dbs_map;

MapService service( GetDefaultConfig( ) );

GeoRectangle box(
    GeoCoordinates( 9.4658, 53.3921 ),
    GeoCoordinates( 10.3233, 53.6582 ) );

const auto result = service.GetLayersForRectangle( box );
```

Trigger incremental map update. It will update onboard map from current version to versoin 7

```cpp
#include <dbs-map-api/MapService.h>
#include <dbs-map-api/MapServiceConfig.h>

using namespace dbs_map;

MapService service( GetDefaultConfig( ) );
service.UpdateMap( 7 );
```
