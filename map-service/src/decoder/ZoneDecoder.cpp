/*
 * SPDX-FileCopyrightText: Copyright DB Netz AG
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ZoneDecoder.h"

#include <decoder/LayerDecoder.h>
#include <map-service/MapServiceConfig.h>

// #include "org/db/schema/f/map/landmarks/geometry.pb.h"
#include "org/db/schema/risk/assessment/zones/zones.pb.h"

namespace map_service
{
namespace decoder
{
using namespace model;
using namespace utils::geo;
namespace proto = org::db::schema::risk::assessment::zones;

namespace
{
const auto zone_type = []( const auto& src )
{
    switch ( src.type( ) )
    {
        case proto::Unknown:
        case proto::ZoneType_INT_MIN_SENTINEL_DO_NOT_USE_:
        case proto::ZoneType_INT_MAX_SENTINEL_DO_NOT_USE_:
            return Zone::Unknown;

        case proto::RiskZone:
            return Zone::RiskZone;

        case proto::SafeZone:
            return Zone::SafeZone;

        case proto::LevelCrossingZone:
            return Zone::LevelCrossingZone;
    };
    return Zone::Unknown;
};

const auto to_zone = []( const auto& src )
{
    return std::make_shared< Zone >( Zone{
        zone_type( src ),
        src.id( ),
        get_polygon_3d( src.geometry( ) ) } );
};
const auto get_zone_id = []( const auto& item )
{
    return item.id( );
};

} // unnamed namespace

ZoneDecoder::ZoneDecoder( const MapServiceConfig& config )
: decoder_( std::make_unique< LayerDecoder >( config, config.catalog_, config.layer_zones_ ) )
{
}

ZoneDecoder::~ZoneDecoder( ) = default;

std::vector< model::Zone::Ptr >
ZoneDecoder::ConstructLayer( const std::vector< PartitionId >& tile_ids ) const
{
    Intersections intersections;
    std::vector< Zone::Ptr > result;
    for ( const auto& tile : decoder_->DecodeTiles< proto::RiskAssessmentZonesTile >( tile_ids ) )
    {
        TransformCollection( tile.second->zones( ), result, to_zone );
        CollectRefs( tile.second->zones_refs( ), intersections );
    }

    for ( const auto& id : tile_ids )
    {
        intersections.erase( id );
    }

    for ( const auto& decoded : decoder_->DecodeIntersections< proto::RiskAssessmentZonesTile >( intersections ) )
    {
        ResolveRefs( decoded.second->zones( ), result,
            intersections[ decoded.first ], get_zone_id, to_zone );
    }

    return result;
}
} // namespace decoder
} // namespace map_service
