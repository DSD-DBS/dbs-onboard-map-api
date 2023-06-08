# Map-Service
Map-Service is a robust C++ API engineered to enable seamless interaction with onboard digital map. It provides a framework for accessing map data stored directly on the device, bypassing the need for constant internet connectivity. With its onboard-centric design, it ensures efficient and reliable map data handling even in the most internet-limited environments.

In addition to its onboard-centric design, it provides the capacity to update maps with the latest data fetched from DB cloud.

## Features
* C++ API: It ensures real-time access to onboard map data: by area and by layers. This means you can selectively retrieve the portions of the map you're interested in, or focus on specific layers of information, offering increased flexibility in managing and utilizing map data.

* Over-the-Air Incremental Updates: One of the distinguishing features of Map-Service is its ability to perform Over-the-Air (OTA) incremental updates. This means that it can fetch and integrate just the changed parts of the map from the cloud, bypassing the need to download the entire map again. Map is hosted in DB intranet and users need to have access to DB intranet to update the map.

## Licenses
This project is compliant with the `REUSE Specification Version 3.0`__.

__ https://git.fsfe.org/reuse/docs/src/commit/d173a27231a36e1a2a3af07421f5e557ae0fec46/spec.md

Copyright DB Netz AG, licensed under Apache 2.0 (see full text in
`<LICENSES/Apache-2.0.txt>`__)

Dot-files are licensed under CC0-1.0 (see full text in
`<LICENSES/CC0-1.0.txt>`__)


## Build

### Prerequisites
* A modern C++ compiler (C++17 or later is recommended).
* CMake (3.16 or later).
* Google Protobuf
* Google Test & Mock
* Boost 1.65.0 or higher
* Libcurl
* OpenSSL
* RapidJSON

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
./build/map-service/tests/GoogleTestRunner
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
using namespace map_service;

MapService service( GetDefaultConfig( ) );

GeoRectangle box(
    GeoCoordinates( 9.4658, 53.3921 ),
    GeoCoordinates( 10.3233, 53.6582 ) );

const auto result = service.GetLayersForRectangle( box );
```

Trigger incremental map update. It will update onboard map from current version to versoin 7

```cpp
using namespace map_service;

MapService service( GetDefaultConfig( ) );
service.UpdateMap( 7 );
```
