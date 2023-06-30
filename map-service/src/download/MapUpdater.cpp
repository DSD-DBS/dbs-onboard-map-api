/*
 * SPDX-FileCopyrightText: Copyright DB Netz AG
 * SPDX-License-Identifier: Apache-2.0
 */

#include "MapUpdater.h"

#include <map-service/download/LayerClient.h>
#include <map-service/download/CatalogClient.h>
#include <map-service/download/ClientSettings.h>
#include <map-service/utils/MapFileSystem.h>
#include <map-service/MapServiceConfig.h>

#include <utils/Hash.h>

#include <boost/range/algorithm/set_algorithm.hpp>

#include <algorithm>
#include <iostream>
#include <fstream>

namespace map_service
{
namespace download
{
namespace
{
namespace fs = std::filesystem;
using Partition = map_service::download::model::Partition;
using LayerClient = map_service::download::LayerClient;
using MapFileSystem = map_service::utils::MapFileSystem;

bool IsPartitionExist( const fs::path& dir, const Partition& metadata )
{
    const auto partition_path = dir / metadata.id_;
    bool result = std::filesystem::exists( partition_path );

    if ( result && metadata.data_size_ > 0 )
    {
        result = fs::file_size( partition_path ) == metadata.data_size_;
        if ( !result )
        {
            std::cerr << "Partition size: " << fs::file_size( partition_path ) << " does not match to metadata: " << metadata.data_size_ << std::endl;
        }
    }
    if ( result && metadata.check_sum_ != 0 )
    {
        result = utils::hash::FileHash_Murmur3_x86_32( partition_path ) == metadata.check_sum_;
        if ( !result )
        {
            std::cerr << "Partition hash: " << utils::hash::FileHash_Murmur3_x86_32( partition_path ) << " does not match to metadata: " << metadata.check_sum_ << std::endl;
        }
    }
    return result;
}

void
DownloadPartition( const LayerClient& client, const fs::path& dst_dir, const Partition& metadata )
{
    fs::path partition_path = dst_dir / metadata.id_;

    if ( fs::remove( partition_path ) )
    {
        std::cout << "Delete partition with incorrect data size or hash." << partition_path << std::endl;
    }

    std::ofstream tile_stream( partition_path );
    if ( !tile_stream.is_open( ) )
    {
        // TODO: exception ?
        std::cerr << "cannot open: " << partition_path << std::endl;
    }

    client.GetDataHandle( metadata.data_handle_, tile_stream );
    tile_stream.close( );

    if ( !IsPartitionExist( dst_dir, metadata ) )
    {
        std::cerr << "Downloaded partition has incorrect hash or data size" << std::endl;
        // TODO: exception ?
    }
}

bool less( const Partition& left, const Partition& right )
{
    return left.catalog_id_ < right.catalog_id_ &&
           left.id_ < right.id_ &&
           left.catalog_version_ < right.catalog_version_ &&

           left.data_size_ < right.data_size_ &&
           left.check_sum_ < right.check_sum_ &&
           left.modification_date_ < right.modification_date_ &&
           left.data_handle_ < right.data_handle_;
}


} // unnamed namespace

MapUpdater::MapUpdater(
    const std::shared_ptr< MapServiceConfig >& config,
    const std::shared_ptr< utils::MapFileSystem >& map_fs )
: config_( config ), map_fs_( map_fs )
{
}

MapUpdater::~MapUpdater( )
{
}

Version
MapUpdater::GetLocalMapVersion( ) const
{
    return map_fs_->GetCurrentMapVersion( );
}

void MapUpdater::CleanLocalCache( ) const
{
    const auto catalog_path = utils::MapFileSystem( config_->map_local_path_, config_->catalog_ ).GetCatalogPath( );

    std::filesystem::remove_all( catalog_path );
    std::filesystem::remove( catalog_path );
}

void MapUpdater::UpdateLocalMap( const Version& to_version ) const
{
    download::CatalogClient catalog_client( config_->catalog_, config_->http_client_settings_ );

    const auto response = catalog_client.GetMetadata( to_version );

    const auto& layer_list = response.layers_;
    for ( const auto& layer : layer_list )
    {
        UpdateLayer( layer, to_version );
    }

    const auto old_target = map_fs_->GetCatalogVersionPath( GetLocalMapVersion( ) );
    const auto target = map_fs_->GetCatalogVersionPath( to_version );
    const auto link = map_fs_->GetCurrentMapPath( );

    std::filesystem::remove( link );

    const auto target_relative_path = std::filesystem::relative( target, link.parent_path( ) );
    std::filesystem::create_symlink( target_relative_path, link );

    if ( old_target != target )
    {
        std::filesystem::remove_all( old_target );
        std::filesystem::remove( old_target );
    }
}

void MapUpdater::UpdateLayer( const std::string& layer_name, const Version& to_version ) const
{
    using namespace map_service::download;
    using namespace utils;

    const auto from_version = GetLocalMapVersion( );
    const auto src_path = map_fs_->GetPartitionFolderPath( from_version, layer_name );
    const auto dst_path = map_fs_->GetPartitionFolderPath( to_version, layer_name );

    std::filesystem::create_directories( dst_path );

    LayerClient client( config_->catalog_, layer_name, to_version, config_->http_client_settings_ );

    const auto response = fs::exists( src_path ) && from_version < to_version ? client.GetDifference( from_version ) : client.GetAllPartitionsMetadata( );
    for ( const auto& partition_meta : response )
    {
        if ( IsPartitionExist( dst_path, partition_meta ) )
        {
            std::cout << " exists at dst: " << dst_path << "/" << partition_meta.id_ << std::endl;
            continue;
        }
        DownloadPartition( client, dst_path, partition_meta );
    }

    const auto all_partitions_response = client.GetAllPartitionsMetadata( );

    std::vector< model::Partition > old_tiles;
    boost::range::set_difference( all_partitions_response, response, std::back_inserter( old_tiles ), less );

    for ( const auto& partition_meta : old_tiles )
    {
        if ( IsPartitionExist( dst_path, partition_meta ) )
        {
            std::cout << " exists at dst: " << dst_path << "/" << partition_meta.id_ << std::endl;
            continue;
        }
        if ( IsPartitionExist( src_path, partition_meta ) )
        {
            std::cout << " exists at src: " << src_path << "/" << partition_meta.id_ << std::endl;
            std::filesystem::copy( src_path / partition_meta.id_, dst_path );
        }
        else
        {
            DownloadPartition( client, src_path, partition_meta );
        }
    }
}
} // namespace download
} // namespace map_service
