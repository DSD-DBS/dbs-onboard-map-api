#pragma once
#include <map-service/model/ConsolidatedLayers.h>
#include <map-service/download/Response.h>

namespace map_service
{
struct MapServiceConfig;

namespace utils
{
class MapFileSystem;
} // namespace utils

namespace decoder
{

template < class T >
class BaseDecoder;

} // namespace decoder

class MapUpdater;

class MapServiceImpl
{
public:
    using Error = map_service::download::Error;

    MapServiceImpl( const MapServiceConfig& config );
    virtual ~MapServiceImpl( );

    virtual Version GetLocalMapVersion( ) const;
    virtual Version GetCloudMapVersion( ) const;

    virtual model::ConsolidatedLayers::Ptr GetLayersForRectangle( const GeoRectangle& rectange ) const;
    virtual model::ConsolidatedLayers::Ptr GetLayersForTiles( const std::vector< PartitionId >& tile_ids ) const;
    virtual model::ConsolidatedLayers::Ptr GetLayersForCorridor( const std::vector< GeoCoordinates >& polyline, double corridor_width_in_meters ) const;

    virtual model::RcaTopology::Ptr GetRcaTopology( const std::vector< PartitionId >& tile_ids ) const;
    virtual std::vector< model::Landmark::Ptr > GetLandmarks( const std::vector< PartitionId >& tile_ids ) const;
    virtual std::vector< model::Zone::Ptr > GetZones( const std::vector< PartitionId >& tile_ids ) const;

    void CleanLocalCache( ) const;
    std::vector< Error > UpdateMap( const Version& version ) const;

private:
    std::shared_ptr< MapServiceConfig > config_;
    std::shared_ptr< decoder::BaseDecoder< model::RcaTopology::Ptr > > topology_decoder_;
    std::shared_ptr< decoder::BaseDecoder< std::vector< model::Landmark::Ptr > > > landmarks_decoder_;
    std::shared_ptr< decoder::BaseDecoder< std::vector< model::Zone::Ptr > > > zones_decoder_;
    std::shared_ptr< utils::MapFileSystem > map_fs_;
    std::shared_ptr< MapUpdater > map_updater_;
};
} // namespace map_service