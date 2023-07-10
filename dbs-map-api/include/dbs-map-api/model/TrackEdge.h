/*
 * SPDX-FileCopyrightText: Copyright DB Netz AG
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#include <dbs-map-api/CommonTypes.h>
#include <dbs-map-api/model/TrackNode.h>

namespace map_service
{
namespace model
{
/**
 * @struct TrackEdge
 * @brief Data structure representing an edge in a railway track graph.
 *
 * This structure defines a TrackEdge object which represents a specific edge or
 * connection between nodes in a railway track graph. Each edge is defined by its unique
 * identifier (ID), the nodes it connects (side_a and side_b), and its spatial geometry.
 */
struct TrackEdge
{
    /**
     * @typedef Ptr
     * @brief Defines a shared pointer to a constant TrackEdge.
     */
    using Ptr = std::shared_ptr< const TrackEdge >;

    /**
     * @typedef MutablePtr
     * @brief Defines a shared pointer to a mutable TrackEdge.
     */
    using MutablePtr = std::shared_ptr< TrackEdge >;

    /**
     * @brief The ID of the track edge.
     *
     * This is the unique identifier of the track edge.
     */
    Id id_;

    /**
     * @brief Pointer to the node at one end of the edge.
     *
     * This is a shared pointer to a TrackNode that represents one of the nodes
     * at the ends of the edge.
     */
    TrackNode::Ptr side_a_;

    /**
     * @brief Pointer to the node at the other end of the edge.
     *
     * This is a shared pointer to a TrackNode that represents one of the nodes
     * at the ends of the edge.
     */
    TrackNode::Ptr side_b_;

    /**
     * @brief The 3D geometry of the track edge.
     *
     * This defines the spatial geometry of the edge in a 3D coordinate system.
     */
    Polyline3d geometry_;
};

} // namespace model
} // namespace map_service
