/*
 * SPDX-FileCopyrightText: Copyright DB Netz AG
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#include <dbs-map-api/CommonTypes.h>
#include <variant>
#include "SimplifiedLandmarks.h"

namespace dbs_map
{
namespace model
{
/**
 * @struct SurfaceWithHeight
 * @brief Data structure representing a 3D surface with a specified height.
 *
 * This structure defines a SurfaceWithHeight object which represents a 3-dimensional
 * polygonal surface with an associated height.
 */
struct SurfaceWithHeight
{
    /**
     * @brief The 3D surface of the object.
     *
     * This defines the spatial geometry of the surface in a 3D coordinate system.
     */
    Polygon3d surface_;

    /**
     * @brief The height of the surface in centimeters.
     *
     * This is the height of the surface above the reference plane, measured in centimeters.
     */
    std::uint64_t height_cm;
};

/**
 * @struct Landmark
 * @brief Data structure representing a landmark in the railway environment.
 *
 * This structure defines a Landmark object which represents a specific landmark in
 * the railway environment. Each Landmark object is defined by its type, a unique
 * identifier (ID), and its spatial geometry.
 */
struct Landmark
{
    /**
     * @typedef Ptr
     * @brief Defines a shared pointer to a constant Landmark.
     */
    using Ptr = std::shared_ptr< const Landmark >;

    /**
     * @typedef MutablePtr
     * @brief Defines a shared pointer to a mutable Landmark.
     */
    using MutablePtr = std::shared_ptr< Landmark >;

    /**
     * @typedef Geometry
     * @brief Defines a variant type that can be one of three types of geometry.
     *
     * This variant type can hold a Polyline3d, Polygon3d, or SurfaceWithHeight object.
     */
    using Geometry = std::variant< Polyline3d, Polygon3d, SurfaceWithHeight >;

    /**
     * @enum Type
     * @brief Enumeration defining types of landmarks.
     *
     * This enumeration defines the types of landmarks that can exist in a railway environment.
     */
    enum Type
    {
        Unknown = 0,
        CatenaryPole,
        Platform,
        Wall,
        SignPole,
        SignalPole,
        BufferStop,
        Billboard,
        BillboardPole,
        BridgePillar,
        Building,
        CameraPole,
        Container,
        CoveredPlatform,
        FuseBox,
        LightPole,
        OtherPole,
        Shelter,
        TrashCan,
        TunnelWall,
        BarrierBar,
        BarrierBody,
    };

    /**
     * @brief The type of the landmark.
     *
     * This is the type of the landmark, as defined in the Type enumeration.
     */
    Type type_;

    /**
     * @brief The ID of the landmark.
     *
     * This is the unique identifier of the landmark.
     */
    Id id_;

    /**
     * @brief The geometry of the landmark.
     *
     * This is the spatial geometry of the landmark, which can be a polyline, a polygon, or a surface with a specified height.
     */
    Geometry geometry_;
};

} // namespace model
} // namespace dbs_map
