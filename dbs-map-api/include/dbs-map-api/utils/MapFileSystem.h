/*
 * SPDX-FileCopyrightText: Copyright DB Netz AG
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#include <dbs-map-api/CommonTypes.h>
#include <filesystem>

namespace map_service
{
namespace utils
{
/**
 * @class MapFileSystem
 * @brief This class encapsulates the management of internal map paths, and the structure of internal map files and folders.
 * It provides interfaces to fetch paths to the catalog, layers, and partitions in both the current map version and a specific version.
 * It also provides a method to retrieve the current map version.
 */
class MapFileSystem
{
public:
    /**
     * @brief Constructs a new MapFileSystem object.
     * @param map_root Path to root directory where all maps and catalogs are stored.
     * @param catalog_name Name of the catalog.
     */
    MapFileSystem( const std::string& map_root, const std::string& catalog_name );

    /**
     * @brief Returns the path to the catalog.
     * @return Path to the catalog.
     */
    std::filesystem::path GetCatalogPath( ) const;

    /**
     * @brief Returns the path to the current map version.
     * @return Path to the current map.
     */
    std::filesystem::path GetCurrentMapPath( ) const;

    /**
     * @brief Returns the path to a specific layer in the current map version.
     * @param layer_name Name of the layer.
     * @return Path to the layer.
     */
    std::filesystem::path GetCurrentMapLayerPath( const std::string& layer_name ) const;

    /**
     * @brief Returns the path to the folder containing partitions for a specific layer in the current map version.
     * @param layer_name Name of the layer.
     * @return Path to the partition folder.
     */
    std::filesystem::path GetCurrentMapPartitionFolderPath( const std::string& layer_name ) const;

    /**
     * @brief Returns the path to a specific partition in a specific layer in the current map version.
     * @param layer_name Name of the layer.
     * @param partition_id ID of the partition.
     * @return Path to the partition.
     */
    std::filesystem::path GetCurrentMapPartitionPath( const std::string& layer_name, PartitionId partition_id ) const;

    /**
     * @brief Returns the path to the catalog for a specific map version.
     * @param version Version of the map.
     * @return Path to the catalog.
     */
    std::filesystem::path GetCatalogVersionPath( const Version& version ) const;

    /**
     * @brief Returns the path to a specific layer for a specific map version.
     * @param version Version of the map.
     * @param layer_name Name of the layer.
     * @return Path to the layer.
     */
    std::filesystem::path GetLayerPath( const Version& version, const std::string& layer_name ) const;

    /**
     * @brief Returns the path to the folder containing partitions for a specific layer in a specific map version.
     * @param version Version of the map.
     * @param layer_name Name of the layer.
     * @return Path to the partition folder.
     */
    std::filesystem::path GetPartitionFolderPath( const Version& version, const std::string& layer_name ) const;

    /**
     * @brief Returns the path to a specific partition in a specific layer for a specific map version.
     * @param version Version of the map.
     * @param layer_name Name of the layer.
     * @param partition_id ID of the partition.
     * @return Path to the partition.
     */
    std::filesystem::path GetPartitionPath( const Version& version, const std::string& layer_name, PartitionId partition_id ) const;

    /**
     * @brief Returns the current map version.
     * @return Current map version.
     */
    Version GetCurrentMapVersion( ) const;

private:
    std::filesystem::path map_root_; // Path to root folder where all maps / catalogs are stored.
    std::string catalog_name_;
};
} // namespace utils
} // namespace map_service
