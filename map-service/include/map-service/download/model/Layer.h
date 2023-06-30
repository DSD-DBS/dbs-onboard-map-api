/*
 * SPDX-FileCopyrightText: Copyright DB Netz AG
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#include <vector>
#include <string>

namespace map_service
{
namespace download
{
namespace model
{
/**
 * @struct Layer
 * @brief Data Transfer Object (DTO) representing metadata for a layer in a map catalog.
 *
 * This structure is used to transfer metadata about a specific layer within a map catalog across
 * different parts of the system or network. The metadata includes the IDs of the catalog and layer,
 * zoom levels, schema name, partitioning type, content type and encoding, and description of the layer.
 */
struct Layer
{
    /**
     * @enum PartitioningType
     * @brief Enumeration defining types of partitioning for a layer.
     *
     * This enumeration defines the types of partitioning that can be used for a layer in a map catalog.
     * The partitioning type determines how the data in the layer is divided and organized.
     */
    enum PartitioningType
    {
        Unknown = 0, ///< Unknown partitioning type.
        HereTiling, ///< HERE tiling partitioning.
    };

    /**
     * @brief The ID of the catalog containing the layer.
     *
     * This string represents the unique identifier for the catalog that contains the layer.
     */
    std::string catalogId_;

    /**
     * @brief The ID of the layer.
     *
     * This string represents the unique identifier for the layer within the catalog.
     */
    std::string layerId_;

    /**
     * @brief The zoom levels available in the layer.
     *
     * This vector contains the zoom levels that are available in the layer. Each zoom level
     * represents a different size of partition (tiling).
     */
    std::vector< std::int32_t > zoom_levels_;

    /**
     * @brief The schema name of the layer.
     *
     * This string represents the name of the schema that is used to structure the data in the layer.
     */
    std::string schema_name_;

    /**
     * @brief The partitioning type of the layer.
     *
     * This represents the type of partitioning that is used to organize the data in the layer.
     */
    PartitioningType partitioning_;

    /**
     * @brief The content type of the layer.
     *
     * This string represents the MIME type of the data in the layer.
     */
    std::string content_type_;

    /**
     * @brief The content encoding of the layer.
     *
     * This string represents the encoding of the data in the layer.
     */
    std::string content_encoding_;

    /**
     * @brief The description of the layer.
     *
     * This string contains a description of the layer. It provides additional information
     * about the layer that might be useful for users.
     */
    std::string description_;
};

} // namespace model
} // namespace download
} // namespace map_service
