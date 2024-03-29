/*
 * SPDX-FileCopyrightText: Copyright DB Netz AG
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#include <dbs-map-api/CommonTypes.h>
#include <variant>

namespace dbs_map
{
namespace model
{
namespace simplified
{

/**
 * @enum ObjectType
 * @brief Enumeration defining types of objects in the railway system.
 *
 * This enumeration defines the types of objects that can exist in a railway system environment. These types include
 * various elements of infrastructure and equipment that may be encountered along a railway track.
 */
enum ObjectType
{
    Unknown = 0,
    CatenaryPoleObject,
    PlatformObject,
    WallObject,
    SignPoleObject,
    SignalPoleObject,
    HandRailObject,
    TunnelObject,
    BufferStopObject,
    BarrierObject,
    BillboardObject,
    BridgeObject,
    BuildingObject,
    CableDuctObject,
    CameraPoleObject,
    CommunicationPoleObject,
    ContainerObject,
    CoveredPlatformObject,
    FuseBoxObject,
    LightPoleObject,
    OtherPoleObject,
    RadioPoleObject,
    ShelterObject,
    SpeakerPoleObject,
    StairwayObject,
    AdColumnObject,
    BenchObject,
    BoothObject,
    BoxObject,
    ElevatorObject,
    HectometreSignObject,
    MileageStoneObject,
    MonitorBoxObject,
    PlatformDisplayObject,
    RainPipeObject,
    TicketMachineObject,
    TicketValidatorObject,
    TrashCanObject,
    VendingMachineObject,
};

/**
 * @struct Lines
 * @brief Data structure representing line geometry for a specific type of object.
 *
 * This template structure defines a Lines object which represents the line geometry of a specific
 * object type in the railway system. Each Lines object is defined by its unique identifier (ID),
 * the parent object's ID, and a set of 3D polylines.
 */
template < ObjectType T >
struct Lines
{
    /**
     * @typedef Ptr
     * @brief Defines a shared pointer to a constant Lines object.
     */
    using Ptr = std::shared_ptr< const Lines >;

    /**
     * @typedef MutablePtr
     * @brief Defines a shared pointer to a mutable Lines object.
     */
    using MutablePtr = std::shared_ptr< Lines >;

    /**
     * @brief The ID of the Lines object.
     *
     * This is the unique identifier of the Lines object.
     */
    Id id_;

    /**
     * @brief The ID of the parent object.
     *
     * This is the unique identifier of the parent object that the Lines object is associated with.
     */
    Id parent_id_;

    /**
     * @brief The 3D polylines of the Lines object.
     *
     * This defines the spatial geometry of the Lines object in a 3D coordinate system.
     */
    std::vector< Polyline3d > lines_;
};

/**
 * @struct Polygons
 * @brief Data structure representing polygon geometry for a specific type of object.
 *
 * This structure defines a Polygons object which represents the polygon geometry of a specific
 * object type. Each Polygons object is defined by its unique identifier (ID), the parent object's ID,
 * and a collection of 3D polygons.
 */
struct Polygons
{
    /**
     * @typedef Ptr
     * @brief Defines a shared pointer to a constant Polygons object.
     */
    using Ptr = std::shared_ptr< const Polygons >;

    /**
     * @typedef MutablePtr
     * @brief Defines a shared pointer to a mutable Polygons object.
     */
    using MutablePtr = std::shared_ptr< Polygons >;

    /**
     * @brief The ID of the Polygons object.
     *
     * This is the unique identifier of the Polygons object.
     */
    Id id_;

    /**
     * @brief The ID of the parent object.
     *
     * This is the unique identifier of the parent object that the Polygons object is associated with.
     */
    Id parent_id_;

    /**
     * @brief The collection of 3D polygons of the Polygons object.
     *
     * This defines the spatial geometry of the Polygons object in a 3D coordinate system.
     */
    std::vector< Polygon3d > polygons_;
};

/**
 * @struct SinglePolygon
 * @brief Data structure representing a single polygon geometry for a specific type of object.
 *
 * This structure defines a SinglePolygon object which represents a single polygon geometry of a specific
 * object type. Each SinglePolygon object is defined by its unique identifier (ID), the parent object's ID,
 * and a single 3D polygon.
 */
struct SinglePolygon
{
    /**
     * @typedef Ptr
     * @brief Defines a shared pointer to a constant SinglePolygon object.
     */
    using Ptr = std::shared_ptr< const SinglePolygon >;

    /**
     * @typedef MutablePtr
     * @brief Defines a shared pointer to a mutable SinglePolygon object.
     */
    using MutablePtr = std::shared_ptr< SinglePolygon >;

    /**
     * @brief The ID of the SinglePolygon object.
     *
     * This is the unique identifier of the SinglePolygon object.
     */
    Id id_;

    /**
     * @brief The ID of the parent object.
     *
     * This is the unique identifier of the parent object that the SinglePolygon object is associated with.
     */
    Id parent_id_;

    /**
     * @brief The 3D polygon of the SinglePolygon object.
     *
     * This defines the spatial geometry of the SinglePolygon object in a 3D coordinate system.
     */
    Polygon3d polygon_;
};

/**
 * @brief Vertical two-vertex 3D line. One or more.
 *
 */
template < ObjectType T >
using Pole = Lines< T >;

/**
 * @brief Two-vertex 3D line (horizontal). One or more.
 *
 */
template < ObjectType T >
using Bar = Lines< T >;

/**
 * @brief Vertical two-vertex 3D line (or oblique). One or more.
 *
 */
template < ObjectType T >
using Pillar = Lines< T >;

/**
 * @brief  Planar multi-vertex polygons. Multiple.
 *
 */
template < ObjectType T >
using Ground = Polygons;

/**
 * @brief Circular multi-vertex 3D polygons or rectangular four-vertex 3D polygons.
 *
 */
template < ObjectType T >
using Buffer = Polygons;

/**
 * @brief Rectangular four-vertex 3D polygon. One.
 *
 */
template < ObjectType T >
using Crossbar = SinglePolygon;

/**
 * @brief Multiple four-vertex rectangular 3D polygons. Multiple.
 *
 */
template < ObjectType T >
using RackFront = Polygons;

/**
 * @brief Usually two three-vertex or four-vertex 3D polygons.
 *
 */
template < ObjectType T >
using Rack = Polygons;

/**
 * @brief Four-vertex 3D polygon (rectangular). One.
 *
 */
template < ObjectType T >
using Sign = SinglePolygon;

/**
 * @brief Four-vertex rectangular 3D polygon. Multi-vertex rectangular 3D polygon.
 *
 */
template < ObjectType T >
using Body = SinglePolygon;

/**
 * @brief Planar four-vertex 3D polygon. One.
 *
 */
template < ObjectType T >
using Board = SinglePolygon;

/**
 * @brief Multi-vertex 3d polygon.
 *
 */
template < ObjectType T >
using Ceiling = Polygons;

/**
 * @brief Multiple planar multi-vertex 3D polygons. Multiple
 *
 */
template < ObjectType T >
using Cover = Polygons;

/**
 * @brief Eight-vertex planar 3D polygon;
 * Four-vertex planar 3D polygons;
 * Four-vertex 3d polygon, plane or curved.
 */
template < ObjectType T >
using SimpleWall = SinglePolygon;

/**
 * @brief 3D polygon with minimum four vertices. One ore more.
 *
 */
template < ObjectType T >
using Surface = Polygons; // 3D polygon with minimum four vertices

/**
 * @brief Catenary Pole object consists from:
 * Pole: One vertical two-vertex 3D line
 *
 */
using CatenaryPole = Pole< CatenaryPoleObject >;

/**
 * @brief Platform object consists from:
 * Surface: At least one 3D polygon with minimum four vertices
 */
using PlatformSurface = Surface< PlatformObject >;

/**
 * @brief Wall object consists from:
 * Multiple planar four-vertex 3D polygons for standard segments.
 * Occasionally, for more complex wall segments: Multiple planar multi-vertex 3D polygons.
 */
using Wall = SimpleWall< WallObject >;

/**
 * @brief Sign pole object consists from:
 * Pole: One vertical two-vertex 3D line.
 */
using SignPole = Pole< SignPoleObject >;

/**
 * @brief Signal Pole object consists from:
 * Pole: One vertical two-vertex 3D line
 */
using SignalPole = Pole< SignalPoleObject >;

/**
 * @brief Hand Rail object consists from:
 * Bar: At least one two-vertex 3D line.
 * Pole: At least one two-vertex 3D line.
 * Wall: At least one four-vertex 3D polygon.
 *
 */
using HandRailBar = Bar< HandRailObject >;
using HandRailPole = Pole< HandRailObject >;
using HandRailWall = SimpleWall< HandRailObject >;

/**
 * @brief Tunnel object consists from:
 * Wall: Multiple planar multi-vertex polygons.
 * Ground: Multiple planar multi-vertex polygons.
 * Pillar: at least one vertical 3D line.
 *
 */
using TunnelWall = SimpleWall< TunnelObject >;
using TunnelGround = Ground< TunnelObject >;
using TunnelPillar = Pillar< TunnelObject >;

/**
 * @brief Buffer Stop object consists from:
 * Buffer: two circular multi-vertex 3D polygons or two rectangular four-vertex 3D polygons.
 * Crossbar: One rectangular four-vertex 3D polygon.
 * Rack Front: Multiple four-vertex rectangular 3D polygons.
 * Rack: Usually two three-vertex or four-vertex 3D polygons.
 * Sign: One rectangular four-vertex 3D polygon.
 *
 */
using BufferStopBuffer = Buffer< BufferStopObject >;
using BufferStopCrossbar = Crossbar< BufferStopObject >;
using BufferStopRackFront = RackFront< BufferStopObject >;
using BufferStopRack = Rack< BufferStopObject >;
using BufferStopSign = Sign< BufferStopObject >;

/**
 * @brief Barrier object consists from:
 * Bar: One two-vertex horizontal 3D line.
 * Body: One four-vertex rectangular 3D polygon (derived from the four bottom vertices of the complex counterpart).
 *
 */
using BarrierBar = Bar< BarrierObject >;
using BarrierBody = Body< BarrierObject >;

/**
 * @brief Billboard object consists from:
 * Board: One planar four-vertex 3D polygon.
 * Pole: At least one two-vertex vertical 3D line
 *
 */
using BillboardBoard = Board< BillboardObject >;
using BillboardPole = Pole< BillboardObject >;

/**
 * @brief Bridge object consists from:
 * Ceiling: At least one multi-vertex 3d polygon.
 * Pillar: At least one two-vertex vertical or oblique 3d line.
 * Wall: At least one four-vertex 3d polygon, plane or curved.
 *
 */
using BridgeCeiling = Ceiling< BridgeObject >;
using BridgePillar = Pillar< BridgeObject >;
using BridgeWall = SimpleWall< BridgeObject >;

/**
 * @brief Buiding object consists from:
 * Body: One at least four-vertex 3D polygon (derived from the four bottom vertices of the complex counterpart).
 *
 */
using BuildingBody = Body< BuildingObject >;

/**
 * @brief Cable Duct object consists from:
 * Cover: Multiple planar multi-vertex 3D polygons.
 *
 */
using CableDuctCover = Cover< CableDuctObject >;

/**
 * @brief Camera pole object consists from:
 * Pole: One vertical two-vertex 3D line.
 *
 */
using CameraPole = Pole< CameraPoleObject >;

/**
 * @brief Comunication Pole object consists from:
 * Pole: At least one vertical two-vertex 3D line.
 *
 */
using CommunicationPole = Pole< CommunicationPoleObject >;

/**
 * @brief Container object consists from:
 * Body: One at least four-vertex 3D polygon (derived from the four bottom vertices of the complex counterpart).
 *
 */
using ContainerBody = Body< ContainerObject >;

/**
 * @brief Covered Platform object consists from:
 * Pillar: One vertical two-vertex 3D line.
 *
 */
using CoveredPlatformPillar = Pillar< CoveredPlatformObject >;

/**
 * @brief Fuse Box object consists from:
 * Body: One at least four-vertex 3D polygon (derived from the four bottom vertices of the complex counterpart).
 *
 */
using FuseBoxBody = Body< FuseBoxObject >;

/**
 * @brief Light Pole object consists from:
 * Pole: One vertical two-vertex 3D line.
 *
 */
using LightPole = Pole< LightPoleObject >;

/**
 * @brief Pole: Other Pole object consists from:
 * One vertical two-vertex 3D line.
 *
 */
using OtherPole = Pole< OtherPoleObject >;

/**
 * @brief Radio Pole object consists from:
 * Pole: One vertical two-vertex 3D line.
 *
 */
using RadioPole = Pole< RadioPoleObject >;

/**
 * @brief Shelter object consists from:
 * Pole: Multiple vertical two-vertex 3D lines.
 * Wall: At least one eight-vertex planar 3D polygon for the sides w/ entrances.
 * and
 * At least three four-vertex planar 3D polygons for the sides w/o entrances.
 *
 */
using ShelterPole = Pole< ShelterObject >;
using ShelterWall = SimpleWall< ShelterObject >;

/**
 * @brief Speaker Pole object consists from:
 * Pole: One vertical two-vertex 3D line.
 *
 */
using SpeakerPole = Pole< SpeakerPoleObject >;

/**
 * @brief Stairway object consists from:
 * Wall: At least three vertical four-vertex 3D polygons.
 *
 */
using StairwayWall = SimpleWall< StairwayObject >;

/**
 * @brief AdColumn object consists from:
 * Body: One vertical 3D line.
 *
 */
using AdColumnBody = Body< AdColumnObject >;


/**
 * @brief Bench object consists from:
 * Body: One four-vertex rectangular 3D polygon (derived from the four bottom vertices of the complex counterpart).
 *
 */
using BenchBody = Body< BenchObject >;

/**
 * @brief Booth object consists from:
 * Body: One multi-vertex rectangular 3D polygon (derived from the four bottom vertices of the complex counterpart).
 *
 */
using BoothBody = Body< BoothObject >;

/**
 * @brief Box object consists from:
 * Body: One four-vertex rectangular 3D polygon (derived from the four bottom vertices of the complex counterpart).
 *
 */
using BoxBody = Body< BoothObject >;

/**
 * @brief Elevator object consists from:
 * Body: One four-vertex rectangular 3D polygon (derived from the four bottom vertices of the complex counterpart).
 *
 */
using ElevatorBody = Body< ElevatorObject >;


/**
 * @brief Hectometre Sign object consists from:
 * Sign: One four-vertex 3D polygon.
 *
 */
using HectometreSign = Sign< HectometreSignObject >;

/**
 * @brief Mileage Stone object consists from:
 * Body: One four-vertex rectangular 3D polygon (derived from the four bottom vertices of the complex counterpart).
 *
 */
using MileageStoneBody = Body< MileageStoneObject >;

/**
 * @brief Monitor Box object consists from:
 * Body: One four-vertex rectangular 3D polygon (derived from the four bottom vertices of the complex counterpart).
 *
 */
using MonitorBoxBody = Body< MonitorBoxObject >;

/**
 * @brief Platform Display object consists from:
 * Pole: One vertical two-vertex 3D line.
 *
 */
using PlatformDisplayPole = Pole< PlatformDisplayObject >;

/**
 * @brief Rain Pipe object consists from:
 * Pole: One vertical two-vertex 3D line.
 *
 */
using RainPipePole = Pole< RainPipeObject >;

/**
 * @brief Ticket Machine object consists from:
 * Body: One four-vertex rectangular 3D polygon (derived from the four bottom vertices of the complex counterpart).
 *
 */
using TicketMachineBody = Body< TicketMachineObject >;

/**
 * @brief Ticket Validator object consists from:
 * Body: One two-vertex vertical 3D line.
 *
 */
using TicketValidatorBody = Body< TicketValidatorObject >;

/**
 * @brief Trash can object consists from:
 * Body: One four-vertex rectangular 3D polygon (derived from the four bottom vertices of the complex counterpart).
 *
 */
using TrashCanBody = Body< TrashCanObject >;

/**
 * @brief Vending Machine object consists from:
 * Body: One four-vertex rectangular 3D polygon (derived from the four bottom vertices of the complex counterpart).
 *
 */
using VendingMachineBody = Body< VendingMachineObject >;

} // namespace simplified
} // namespace model
} // namespace dbs_map
