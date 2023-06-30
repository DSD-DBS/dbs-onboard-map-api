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
 * @struct TrackNode
 * @brief Data structure representing a node in a railway track graph.
 *
 * This structure defines a TrackNode object which represents a specific node (or point)
 * in a railway track graph. Each node is defined by its unique identifier (ID) and type,
 * which can be one of several predefined types such as Point, SystemBorder, BufferStop, etc.
 */
struct TrackNode
{
    /**
     * @typedef Ptr
     * @brief Defines a shared pointer to a constant TrackNode.
     */
    using Ptr = std::shared_ptr< const TrackNode >;

    /**
     * @typedef MutablePtr
     * @brief Defines a shared pointer to a mutable TrackNode.
     */
    using MutablePtr = std::shared_ptr< TrackNode >;

    /**
     * @enum Type
     * @brief Enumeration defining types of track nodes.
     *
     * This enumeration defines the types of nodes that can exist in a railway track graph.
     */
    enum Type
    {
        Unknown = 0, ///< Unknown node type.
        Point, ///< Point node type, usually representing a connection or junction in the track.
        SystemBorder, ///< System border node type, usually representing an entry or exit point from the railway system.
        BufferStop, ///< Buffer stop node type, usually representing the end of a track.
    };

    /**
     * @brief The ID of the track node.
     *
     * This is the unique identifier of the track node.
     */
    Id id_;

    /**
     * @brief The type of the track node.
     *
     * This represents the type of the track node as defined by the enumeration Type.
     */
    Type type_;
};

} // namespace model
} // namespace map_service
