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
namespace
{
namespace fs = std::filesystem;
using Partition = map_service::download::model::Partition;
using Error = map_service::download::Error;
using ErrorCode = map_service::download::ErrorCode;
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

Error DownloadPartition( const LayerClient& client, const fs::path& dst_dir, const Partition& metadata )
{
    fs::path partition_path = dst_dir / metadata.id_;

    if ( fs::remove( partition_path ) )
    {
        std::cout << "Delete partition with incorrect data size or hash." << partition_path << std::endl;
    }

    std::ofstream tile_stream( partition_path );
    if ( !tile_stream.is_open( ) )
    {
        // TODO: populate error
        std::cerr << "cannot open: " << partition_path << std::endl;
    }
    // TODO: check data size and checksum
    const auto err = client.WriteData( metadata.data_handle_, tile_stream );
    tile_stream.close( );

    if ( !IsPartitionExist( dst_dir, metadata ) )
    {
        std::cerr << "Downloaded partition has incorrect hash or data size" << std::endl;
    }

    if ( err.error_code_ != ErrorCode::Success )
    {
        std::cerr << "cannot download partition: " << metadata.id_ << ", " << metadata.data_handle_ << ", error: " << err.error_code_ << std::endl;
    }

    return err;
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

std::vector< MapUpdater::Error >
MapUpdater::UpdateMap( const Version& to_version ) const
{
    download::CatalogClient catalog_client( config_->catalog_, config_->http_client_settings_ );

    const auto response = catalog_client.GetMetadata( to_version );
    if ( response.error_.error_code_ != ErrorCode::Success )
    {
        return { response.error_ };
    }

    const auto& layer_list = response.content_.layers_;
    for ( const auto& layer : layer_list )
    {
        const auto errors = UpdateLayer( layer, to_version );
        if ( !errors.empty( ) )
        {
            return errors;
        }
    }

    const auto old_target = map_fs_->GetCatalogVersionPath( GetLocalMapVersion( ) );
    const auto target = map_fs_->GetCatalogVersionPath( to_version );
    const auto link = map_fs_->GetCurrentMapPath( );

    // std::filesystem::remove_all( link );
    std::filesystem::remove( link );
    std::filesystem::create_symlink( target, link );

    if ( old_target != target )
    {
        std::filesystem::remove_all( old_target );
        std::filesystem::remove( old_target );
    }

    return { };
}

std::vector< Error >
MapUpdater::UpdateLayer( const std::string& layer_name, const Version& to_version ) const
{
    using namespace map_service::download;
    using namespace utils;

    std::vector< Error > errors;

    const auto from_version = GetLocalMapVersion( );

    const auto src_path = map_fs_->GetPartitionFolderPath( from_version, layer_name );
    const auto dst_path = map_fs_->GetPartitionFolderPath( to_version, layer_name );

    std::filesystem::create_directories( dst_path );

    LayerClient client( config_->catalog_, layer_name, to_version, config_->http_client_settings_ );

    const auto response = fs::exists( src_path ) && from_version < to_version ? client.GetDifference( from_version ) : client.GetAllPartitionsMetadata( );

    if ( response.error_.error_code_ != ErrorCode::Success )
    {
        errors.emplace_back( response.error_ );
        return errors;
    }
    for ( const auto& partition_meta : response.content_ )
    {
        if ( IsPartitionExist( dst_path, partition_meta ) )
        {
            std::cout << " exists at dst: " << dst_path << "/" << partition_meta.id_ << std::endl;
            continue;
        }

        const auto error = DownloadPartition( client, dst_path, partition_meta );
        if ( error.error_code_ != ErrorCode::Success )
        {
            errors.emplace_back( error );
            continue;
        }
    }

    const auto all_partitions_response = client.GetAllPartitionsMetadata( );
    if ( all_partitions_response.error_.error_code_ != ErrorCode::Success )
    {
        errors.emplace_back( all_partitions_response.error_ );
        return errors;
    }

    PartitionListResponse::Content old_tiles;
    boost::range::set_difference( all_partitions_response.content_, response.content_, std::back_inserter( old_tiles ), less );

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
            const auto error = DownloadPartition( client, src_path, partition_meta );
            if ( error.error_code_ != ErrorCode::Success )
            {
                errors.emplace_back( error );
            }
        }
    }

    return errors;
}
} // namespace map_service
