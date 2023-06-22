/*
 * SPDX-FileCopyrightText: Copyright DB Netz AG
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#include <map-service/CommonTypes.h>
#include <filesystem>

namespace map_service
{
namespace utils
{
class MapFileSystem
{
public:
    MapFileSystem( const std::string& map_root, const std::string& catalog_name );

    std::filesystem::path GetCatalogPath( ) const;

    std::filesystem::path GetCurrentMapPath( ) const;
    std::filesystem::path GetCurrentMapLayerPath( const std::string& layer_name ) const;
    std::filesystem::path GetCurrentMapPartitionFolderPath( const std::string& layer_name ) const;
    std::filesystem::path GetCurrentMapPartitionPath( const std::string& layer_name, PartitionId partition_id ) const;

    std::filesystem::path GetCatalogVersionPath( const Version& version ) const;
    std::filesystem::path GetLayerPath( const Version& version, const std::string& layer_name ) const;
    std::filesystem::path GetPartitionFolderPath( const Version& version, const std::string& layer_name ) const;
    std::filesystem::path GetPartitionPath( const Version& version, const std::string& layer_name, PartitionId partition_id ) const;

    Version GetCurrentMapVersion( ) const;

private:
    std::filesystem::path map_root_; // Path to root folder where all maps / catalogs are stored.
    std::string catalog_name_;
};
} // namespace utils
} // namespace map_service
