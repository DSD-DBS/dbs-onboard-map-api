/*
 * SPDX-FileCopyrightText: Copyright DB Netz AG
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#include <dbs-map-api/model/TrackNode.h>
#include <dbs-map-api/model/TrackEdge.h>

namespace dbs_map
{
namespace model
{
/**
 * @struct RcaTopology
 * @brief Data structure representing the topology of a railway system.
 *
 * This structure defines an RcaTopology object which encapsulates the entire topology
 * of a railway system. The topology is represented as a graph with nodes and edges,
 * where each node is a point on the track and each edge is a connection between two points.
 */
struct RcaTopology
{
    /**
     * @typedef Ptr
     * @brief Defines a shared pointer to a constant RcaTopology.
     */
    using Ptr = std::shared_ptr< const RcaTopology >;

    /**
     * @typedef MutablePtr
     * @brief Defines a shared pointer to a mutable RcaTopology.
     */
    using MutablePtr = std::shared_ptr< RcaTopology >;

    /**
     * @brief Vector of pointers to TrackNode objects.
     *
     * This vector holds shared pointers to all the TrackNode objects that form the nodes
     * of the railway track graph.
     */
    std::vector< TrackNode::Ptr > nodes_;

    /**
     * @brief Vector of pointers to TrackEdge objects.
     *
     * This vector holds shared pointers to all the TrackEdge objects that form the edges
     * of the railway track graph.
     */
    std::vector< TrackEdge::Ptr > edges_;
};

} // namespace model
} // namespace dbs_map
