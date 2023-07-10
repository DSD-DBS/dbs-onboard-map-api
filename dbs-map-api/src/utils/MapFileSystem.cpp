/*
 * SPDX-FileCopyrightText: Copyright DB Netz AG
 * SPDX-License-Identifier: Apache-2.0
 */

#include <dbs-map-api/utils/MapFileSystem.h>

namespace dbs_map
{
namespace utils
{

MapFileSystem::MapFileSystem( const std::string& map_root, const std::string& catalog_name )
: map_root_( map_root ), catalog_name_( catalog_name )
{
}

std::filesystem::path
MapFileSystem::GetCatalogPath( ) const
{
    auto result = map_root_;
    return result.append( catalog_name_ );
}
std::filesystem::path
MapFileSystem::GetCurrentMapPath( ) const
{
    return GetCatalogPath( ).append( "current" );
}

std::filesystem::path
MapFileSystem::GetCurrentMapLayerPath( const std::string& layer_name ) const
{
    return GetCurrentMapPath( ).append( layer_name );
}

std::filesystem::path
MapFileSystem::GetCurrentMapPartitionFolderPath( const std::string& layer_name ) const
{
    return GetCurrentMapLayerPath( layer_name ).append( "partitions" );
}

std::filesystem::path
MapFileSystem::GetCurrentMapPartitionPath( const std::string& layer_name, PartitionId partition_id ) const
{
    return GetCurrentMapPartitionFolderPath( layer_name ).append( partition_id );
}

std::filesystem::path
MapFileSystem::GetCatalogVersionPath( const Version& version ) const
{
    return GetCatalogPath( ).append( std::to_string( version ) );
}

std::filesystem::path
MapFileSystem::GetLayerPath( const Version& version, const std::string& layer_name ) const
{
    return GetCatalogVersionPath( version ).append( layer_name );
}

std::filesystem::path
MapFileSystem::GetPartitionFolderPath( const Version& version, const std::string& layer_name ) const
{
    return GetLayerPath( version, layer_name ).append( "partitions" );
}

std::filesystem::path
MapFileSystem::GetPartitionPath( const Version& version, const std::string& layer_name, PartitionId partition_id ) const
{
    return GetPartitionFolderPath( version, layer_name ).append( partition_id );
}

Version
MapFileSystem::GetCurrentMapVersion( ) const
{
    namespace fs = std::filesystem;
    Version result = 0;

    const auto catalog_path = GetCurrentMapPath( );
    if ( fs::exists( catalog_path ) && fs::is_symlink( catalog_path ) )
    {
        result = std::stoull( fs::read_symlink( catalog_path ).filename( ) );
    }
    return result;
}

} // namespace utils
} // namespace dbs_map
