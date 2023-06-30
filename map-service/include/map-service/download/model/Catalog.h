/*
 * SPDX-FileCopyrightText: Copyright DB Netz AG
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#include <map-service/CommonTypes.h>
#include <vector>

namespace map_service
{
namespace download
{
namespace model
{
/**
 * @struct Catalog
 * @brief Data Transfer Object (DTO) representing metadata for a map catalog.
 *
 * This structure is used to transfer metadata about a map catalog across different
 * parts of the system or network. The metadata includes the ID, versions, modification date,
 * description, and layers of the catalog.
 */
struct Catalog
{
    /**
     * @brief The ID of the catalog.
     *
     * This string represents the unique identifier for the catalog.
     */
    std::string id_;

    /**
     * @brief The version of the catalog.
     *
     * This represents the version of the catalog. It is specified by the map service.
     */
    map_service::Version version_;

    /**
     * @brief The latest version of the catalog.
     *
     * This represents the latest available version of the catalog. It is specified by the map service.
     */
    map_service::Version latest_version_;

    /**
     * @brief The modification date of the catalog.
     *
     * This is a timestamp (in milliseconds since Unix epoch) representing when the catalog was last modified.
     */
    std::uint64_t modification_date_;

    /**
     * @brief The description of the catalog.
     *
     * This string contains a description of the catalog. It provides additional information
     * about the catalog that might be useful for users.
     */
    std::string description_;

    /**
     * @brief The layers in the catalog.
     *
     * This vector contains the names of the layers that are part of the catalog.
     */
    std::vector< std::string > layers_;
};

} // namespace model
} // namespace download
} // namespace map_service
