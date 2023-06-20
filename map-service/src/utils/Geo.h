/*
 * SPDX-FileCopyrightText: Copyright DB Netz AG
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#include <map-service/CommonTypes.h>
#include <unordered_map>
#include <unordered_set>


namespace map_service
{
namespace utils
{
namespace geo
{

using Intersections = std::unordered_map< PartitionId, std::unordered_set< Id > >;

template< class TRef >
void AddRef( const TRef& ref, Intersections& out )
{
    auto inserted = out.emplace( std::to_string( ref.tile_id( ) ), std::unordered_set< Id >( ) );
    inserted.first->second.emplace( ref.id( ) );
}

template < class TRefCollection >
void CollectRefs( const TRefCollection& refs, Intersections& out )
{
    for ( const auto& tiled_ref : refs )
    {
        AddRef( tiled_ref, out );
    }
}

template < class TInput, class TOutput, class TIdGetter, class TTransformer >
void ResolveRefs( const TInput& src,
    TOutput& dst,
    const std::unordered_set< Id >& filter,
    const TIdGetter& getter,
    const TTransformer& fn )
{
    for ( const auto& item : src )
    {
        if ( filter.find( getter( item ) ) != filter.cend( ) )
        {
            dst.emplace_back( fn( item ) );
        }
    }
}

std::vector< PartitionId > ToTileIds( const std::vector< GeoCoordinates >& polyline, double corridor_width_in_meters, std::uint32_t zoom_level );
std::vector< PartitionId > ToTileIds( const GeoRectangle& box, std::uint32_t zoom_level );


template < class Src, class Dst, class Fn >
void TransformCollection( const Src& src, Dst& dst, Fn fn )
{
    dst.reserve( src.size( ) + dst.size( ) );
    std::transform( src.begin( ), src.end( ), std::back_inserter( dst ), fn );
}

template < class TMap >
std::vector< typename TMap::key_type >
GetKeys( const TMap& src )
{
    std::vector< typename TMap::key_type > result;
    result.reserve( src.size( ) );

    std::transform( src.cbegin( ), src.cend( ), std::back_inserter( result ), []( const auto& pair )
        { return pair.first; } );

    return result;
}

const auto to_geo_coordinate_3d = []( const auto& center_point )
{
    return GeoCoordinates3d( center_point.longitude( ), center_point.latitude( ),
        // TODO: check how to convert cm_from_wgs84_ellipsoid needs to be converted --> GeoCoordinates3d::altitude
        center_point.meter_from_wgs84_ellipsoid( ) );
};

const auto get_polyline_3d = []( const auto& src )
{
    const auto& geometry = src.points( );

    Polyline3d result;
    result.reserve( geometry.size( ) );
    std::transform( geometry.begin( ), geometry.end( ), std::back_inserter( result ), to_geo_coordinate_3d );

    return result;
};

const auto get_polygon_3d = []( const auto& src )
{
    Polygon3d result;
    boost::geometry::assign_points( result, get_polyline_3d( src ) );

    return result;
};

} // namespace geo
} // namespace utils
} // namespace map_service
