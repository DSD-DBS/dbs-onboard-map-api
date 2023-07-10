/*
 * SPDX-FileCopyrightText: Copyright DB Netz AG
 * SPDX-License-Identifier: Apache-2.0
 */

#include "LandmarkDecoder.h"

#include <decoder/LayerDecoder.h>
#include <utils/Geo.h>
#include <dbs-map-api/MapServiceConfig.h>

#include "org/db/schema/f/map/landmarks/geometry.pb.h"
#include "org/db/schema/f/map/landmarks/landmarks.pb.h"

namespace map_service
{

namespace decoder
{
using namespace model;
using namespace utils::geo;

namespace proto = org::db::schema::f::map::landmarks;

namespace
{

std::ostream& operator<<( std::ostream& os, proto::Landmark::GeometryCase geometry_case )
{
    static const char* names[] = {
        "kLine3D",
        "kPolygon3D",
        "kSurfaceWithHeight",
        "kBillboard",
        "GEOMETRY_NOT_SET",
    };
    return os << names[ geometry_case - proto::Landmark::kLine3D ];
}

const auto get_landmark_type = []( const auto& src )
{
    using namespace proto;

    switch ( src.type( ) )
    {
        case CatenaryPole:
            return Landmark::CatenaryPole;
        case Platform:
            return Landmark::Platform;
        case Wall:
            return Landmark::Wall;
        case SignPole:
            return Landmark::SignPole;
        case SignalPole:
            return Landmark::SignalPole;
        case BufferStop:
            return Landmark::BufferStop;
        case Billboard:
            return src.geometry_case( ) == proto::Landmark::kLine3D ? Landmark::BillboardPole : Landmark::Billboard;
        case BridgePillar:
            return Landmark::BridgePillar;
        case Building:
            return Landmark::Building;
        case CameraPole:
            return Landmark::CameraPole;
        case Container:
            return Landmark::Container;
        case CoveredPlatform:
            return Landmark::CoveredPlatform;
        case FuseBox:
            return Landmark::FuseBox;
        case LightPole:
            return Landmark::LightPole;
        case OtherPole:
            return Landmark::OtherPole;
        case Shelter:
            return Landmark::Shelter;
        case TrashCan:
            return Landmark::TrashCan;
        case TunnelWall:
            return Landmark::TunnelWall;
        case BarrierBar:
            return Landmark::BarrierBar;
        case BarrierBody:
            return Landmark::BarrierBody;

        case LandmarkType_INT_MIN_SENTINEL_DO_NOT_USE_:
        case LandmarkType_INT_MAX_SENTINEL_DO_NOT_USE_:
        case Unknown:
            return Landmark::Unknown;
    };
    return Landmark::Unknown;
};

const auto get_geometry = []( const auto& src )
{
    using namespace utils::geo;

    Landmark::Geometry result;
    switch ( src.geometry_case( ) )
    {
        case proto::Landmark::kLine3D:
        {
            result = get_polyline_3d( src.line3d( ) );
            break;
        }
        case proto::Landmark::kPolygon3D:
        {
            result = get_polygon_3d( src.polygon3d( ) );
            break;
        }
        case proto::Landmark::kSurfaceWithHeight:
        {
            result = SurfaceWithHeight{
                get_polygon_3d( src.surface_with_height( ).surface( ) ),
                src.surface_with_height( ).height_cm( ) };
            break;
        }
        default:
            break; // supress warning
    };
    return result;
};

bool detect_error( const proto::Landmark& src, proto::Landmark::GeometryCase expected )
{
    const auto actual = src.geometry_case( );
    if ( actual != expected )
    {
        std::cerr << "Error: Landmark type: " << LandmarkType_Name( src.type( ) )
                  << " should have geometry: " << expected
                  << " actual geometry is: " << actual
                  << " id: " << src.id( ) << std::endl;
        return false;
    }
    return true;
}

const auto check_type_against_geometry = []( const auto& src )
{
    using namespace proto;

    switch ( src.type( ) )
    {
        case CatenaryPole:
        case SignPole:
        case SignalPole:
        case CameraPole:
        case LightPole:
        case OtherPole:
        case BufferStop:
        case BridgePillar:
        case CoveredPlatform:
        case Shelter:
        case BarrierBar:
        {
            return detect_error( src, proto::Landmark::kLine3D );
        }
        case Platform:
        case Container:
        case FuseBox:
        case Building:
        case TrashCan:
        case BarrierBody:
        {
            return detect_error( src, proto::Landmark::kSurfaceWithHeight );
        }
        case Wall:
        case TunnelWall:
        {
            return detect_error( src, proto::Landmark::kPolygon3D );
        }
        case Billboard:
        {
            return detect_error( src, src.geometry_case( ) == proto::Landmark::kPolygon3D ? proto::Landmark::kPolygon3D : proto::Landmark::kLine3D );
        }
        case LandmarkType_INT_MIN_SENTINEL_DO_NOT_USE_:
        case LandmarkType_INT_MAX_SENTINEL_DO_NOT_USE_:
        case Unknown:
        {
            std::cerr << "Error: Object has Unknown type. ID: " << src.id( ) << std::endl;
            return false;
        }
    };
    return true;
};

const auto get_landmark = []( const auto& src )
{
    return std::make_shared< Landmark >(
        Landmark{
            get_landmark_type( src ),
            src.id( ),
            get_geometry( src ) } );
};

} // unnamed namespace
LandmarksDecoder::LandmarksDecoder( const MapServiceConfig& config )
: decoder_( std::make_unique< LayerDecoder >( config, config.catalog_, config.layer_landmarks_ ) )
{
}

LandmarksDecoder::~LandmarksDecoder( ) = default;

std::vector< model::Landmark::Ptr >
LandmarksDecoder::ConstructLayer( const std::vector< PartitionId >& tile_ids ) const
{
    std::vector< Landmark::Ptr > result;
    for ( const auto& tile : decoder_->DecodeTiles< proto::LandmarkLayerTile >( tile_ids ) )
    {
        result.reserve( tile.second->landmarks( ).size( ) );
        for ( const auto& landmark : tile.second->landmarks( ) )
        {
            if ( check_type_against_geometry( landmark ) )
            {
                result.emplace_back( get_landmark( landmark ) );
            }
        }
        //  TODO: CollectRefs( tile.second->intersecting_zones( ), intersections );
    }

    return result;
}
} // namespace decoder
} // namespace map_service
