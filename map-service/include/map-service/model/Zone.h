/*
 * SPDX-FileCopyrightText: Copyright DB Netz AG
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#include <map-service/CommonTypes.h>

namespace map_service
{
namespace model
{
class Zone
{
public:
    using Ptr = std::shared_ptr< const Zone >;
    using MutablePtr = std::shared_ptr< Zone >;

    enum Type
    {
        Unknown = 0,
        RiskZone,
        SafeZone,
        LevelCrossingZone,
    };

    Type type_;
    Id id_;
    Polygon3d geometry_;
};
} // namespace model
} // namespace map_service
