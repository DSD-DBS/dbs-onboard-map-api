
#include <MapServiceImpl.h>
#include <MapUpdater.h>

#include <decoder/RcaTopologyDecoder.h>
#include <decoder/ZoneDecoder.h>
#include <decoder/LandmarkDecoder.h>

#include <map-service/utils/MapFileSystem.h>
#include <map-service/MapServiceConfig.h>
#include <map-service/download/CatalogClient.h>


#include <utils/Geo.h>
#include <utils/ProtobufCleaner.h>


namespace map_service
{
using namespace decoder;

MapServiceImpl::MapServiceImpl( const MapServiceConfig& config )
: config_( std::make_shared< MapServiceConfig >( config ) ),
  topology_decoder_( std::make_shared< RcaTopologyDecoder >( *config_ ) ),
  landmarks_decoder_( std::make_shared< LandmarksDecoder >( *config_ ) ),
  zones_decoder_( std::make_shared< ZoneDecoder >( *config_ ) ),
  map_fs_( std::make_shared< utils::MapFileSystem >( config_->map_local_path_, config_->catalog_ ) ),
  map_updater_( std::make_shared< MapUpdater >( config_, map_fs_ ) )
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
    if ( map_updater_ )
    {
        map_updater_->CleanLocalCache( );
    }
}

std::vector< MapServiceImpl::Error >
MapServiceImpl::UpdateMap( const Version& to_version ) const
{
    return map_updater_ ? map_updater_->UpdateMap( to_version ) : std::vector< Error >( );
}
} // namespace map_service