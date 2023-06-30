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
/**
 * @class Zone
 * @brief Class representing a specific zone for risk assessment.
 *
 * This class defines a Zone object which represents an area in which a specific risk assessment is done.
 * These zones can be of different types such as RiskZone, SafeZone, LevelCrossingZone, etc. Each zone is defined
 * by its type, unique identifier (ID), and 3D geometry.
 */
class Zone
{
public:
    /**
     * @typedef Ptr
     * @brief Defines a shared pointer to a constant Zone.
     */
    using Ptr = std::shared_ptr< const Zone >;

    /**
     * @typedef MutablePtr
     * @brief Defines a shared pointer to a mutable Zone.
     */
    using MutablePtr = std::shared_ptr< Zone >;

    /**
     * @enum Type
     * @brief Enumeration defining types of zones.
     *
     * This enumeration defines the types of zones that can be used for risk assessment.
     */
    enum Type
    {
        Unknown = 0, ///< Unknown zone type.
        RiskZone, ///< Risk zone type.
        SafeZone, ///< Safe zone type.
        LevelCrossingZone, ///< Level crossing zone type.
    };

    /**
     * @brief The type of the zone.
     *
     * This represents the type of the zone as defined by the enumeration Type.
     */
    Type type_;

    /**
     * @brief The ID of the zone.
     *
     * This is the unique identifier of the zone.
     */
    Id id_;

    /**
     * @brief The 3D geometry of the zone.
     *
     * This defines the spatial geometry of the zone in a 3D coordinate system.
     */
    Polygon3d geometry_;
};

} // namespace model
} // namespace map_service
