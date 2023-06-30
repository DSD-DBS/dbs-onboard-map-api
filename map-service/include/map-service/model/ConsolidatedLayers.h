/*
 * SPDX-FileCopyrightText: Copyright DB Netz AG
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#include <map-service/model/RcaTopology.h>
#include <map-service/model/Landmarks.h>
#include <map-service/model/Zone.h>

namespace map_service
{
namespace model
{
/**
 * @struct ConsolidatedLayers
 * @brief Data structure representing a consolidated view of all map layers.
 *
 * This structure defines a ConsolidatedLayers object which provides a consolidated view of all
 * the map layers in the database. This includes the topology of the railway system, 3D landmarks,
 * risk assessment zones, and the associated data partitions (tiles) representing specific areas of the railway system.
 * Each ConsolidatedLayers object also maintains a list of PartitionIds which are used for efficient data management and querying.
 */
struct ConsolidatedLayers
{
    /**
     * @typedef Ptr
     * @brief Defines a shared pointer to a constant ConsolidatedLayers.
     */
    using Ptr = std::shared_ptr< const ConsolidatedLayers >;

    /**
     * @typedef MutablePtr
     * @brief Defines a shared pointer to a mutable ConsolidatedLayers.
     */
    using MutablePtr = std::shared_ptr< ConsolidatedLayers >;

    /**
     * @brief The topology of the railway system.
     *
     * This is a shared pointer to the RcaTopology object that represents the topology of the railway system.
     */
    RcaTopology::Ptr topology_;

    /**
     * @brief The landmarks in the railway system.
     *
     * This is a vector of shared pointers to the Landmark objects that represent the landmarks in the railway system.
     */
    std::vector< Landmark::Ptr > landmarks_;

    /**
     * @brief The risk assessment zones in the railway system.
     *
     * This is a vector of shared pointers to the Zone objects that represent the risk assessment zones in the railway system.
     */
    std::vector< Zone::Ptr > zones_;

    /**
     * @brief The IDs of the partitions in the database.
     *
     * This is a vector of PartitionId objects. Each PartitionId corresponds to a data partition in the database.
     * These partitions (tiles) represent specific areas of the railway system.
     */
    std::vector< PartitionId > partition_ids_;
};

} // namespace model
} // namespace map_service
