/*
 * SPDX-FileCopyrightText: Copyright DB Netz AG
 * SPDX-License-Identifier: Apache-2.0
 */

#include <MapServiceImpl.h>
#include <download/MapUpdater.h>

#include <decoder/RcaTopologyDecoder.h>
#include <decoder/ZoneDecoder.h>
#include <decoder/LandmarkDecoder.h>

#include <dbs-map-api/utils/MapFileSystem.h>
#include <dbs-map-api/MapServiceConfig.h>
#include <dbs-map-api/download/CatalogClient.h>
#include <dbs-map-api/download/HttpRuntimeError.h>

#include <utils/Geo.h>
#include <utils/ProtobufCleaner.h>

#include <curlpp/Exception.hpp>
#include <iostream>

namespace dbs_map
{
using namespace decoder;

MapServiceImpl::MapServiceImpl( const MapServiceConfig& config )
: config_( std::make_shared< MapServiceConfig >( config ) ),
  topology_decoder_( std::make_shared< RcaTopologyDecoder >( *config_ ) ),
  landmarks_decoder_( std::make_shared< LandmarksDecoder >( *config_ ) ),
  zones_decoder_( std::make_shared< ZoneDecoder >( *config_ ) ),
  map_fs_( std::make_shared< utils::MapFileSystem >( config_->map_local_path_, config_->catalog_ ) ),
  map_updater_( std::make_shared< download::MapUpdater >( config_, map_fs_ ) )
{
    static auto protobuf_cleaner = utils::ProtobufCleaner( );
}

MapServiceImpl::~MapServiceImpl( )
{
}

Version
MapServiceImpl::GetLocalMapVersion( ) const
{
    return map_fs_->GetCurrentMapVersion( );
}

Version
MapServiceImpl::GetCloudMapVersion( ) const
{
    download::CatalogClient catalog_client( config_->catalog_, config_->http_client_settings_ );
    return catalog_client.GetLatestVersion( );
}

// Consolidate area api
model::ConsolidatedLayers::Ptr
MapServiceImpl::GetLayersForRectangle( const GeoRectangle& box ) const
{
    return GetLayersForTiles( utils::geo::ToTileIds( box, config_->zoom_level_ ) );
}

model::ConsolidatedLayers::Ptr
MapServiceImpl::GetLayersForTiles( const std::vector< PartitionId >& tile_ids ) const
{
    using namespace model;

    return std::make_shared< ConsolidatedLayers >( ConsolidatedLayers{
        GetRcaTopology( tile_ids ),
        GetLandmarks( tile_ids ),
        GetZones( tile_ids ),
        tile_ids } );
}
model::ConsolidatedLayers::Ptr
MapServiceImpl::GetLayersForCorridor( const std::vector< GeoCoordinates >& polyline, double corridor_width_in_meters ) const
{
    return GetLayersForTiles( utils::geo::ToTileIds( polyline, corridor_width_in_meters, config_->zoom_level_ ) );
}


model::RcaTopology::Ptr
MapServiceImpl::GetRcaTopology( const std::vector< PartitionId >& tile_ids ) const
{
    return topology_decoder_->ConstructLayer( tile_ids );
}

std::vector< model::Landmark::Ptr >
MapServiceImpl::GetLandmarks( const std::vector< PartitionId >& tile_ids ) const
{
    return landmarks_decoder_->ConstructLayer( tile_ids );
}

std::vector< model::Zone::Ptr >
MapServiceImpl::GetZones( const std::vector< PartitionId >& tile_ids ) const
{
    return zones_decoder_->ConstructLayer( tile_ids );
}

void MapServiceImpl::CleanLocalCache( ) const
{
    map_updater_->CleanLocalCache( );
}

void MapServiceImpl::UpdateLocalMap( const Version& to_version ) const
{
    map_updater_->UpdateLocalMap( to_version );
}

std::vector< MapService::Error >
MapServiceImpl::UpdateMap( const Version& to_version ) const noexcept
{
    using namespace download;

    try
    {
        UpdateLocalMap( to_version );
        return { };
    }
    catch ( const curlpp::LibcurlRuntimeError& e )
    {
        std::cerr << e.what( ) << ", CURL Code: " << e.whatCode( ) << std::endl;
        return { { ErrorCode::CurlError, e.what( ), e.whatCode( ), 200 } };
    }
    catch ( const download::HttpRuntimeError& e )
    {
        std::cerr << e.what( ) << ", HTTP Status Code: " << e.WhatCode( ) << std::endl;
        return { { ErrorCode::HttpError, e.what( ), CURLE_OK, e.WhatCode( ) } };
    }
    catch ( const std::exception& e )
    {
        std::cerr << e.what( ) << std::endl;
        return { { ErrorCode::Unknown, e.what( ) } };
    }
    catch ( ... )
    {
        const auto msg = "Http client unknown exception";
        std::cerr << msg << std::endl;
        return { { ErrorCode::Unknown, msg } };
    }
}
} // namespace dbs_map
