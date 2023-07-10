/*
 * SPDX-FileCopyrightText: Copyright DB Netz AG
 * SPDX-License-Identifier: Apache-2.0
 */

#include "Geo.h"

#include <olp/core/geo/coordinates/GeoCoordinates.h>
#include <olp/core/geo/coordinates/GeoRectangle.h>
#include <olp/core/geo/tiling/TileKeyUtils.h>
#include <olp/core/geo/tiling/TilingSchemeRegistry.h>

#include <boost/geometry/algorithms/assign.hpp>

namespace map_service
{
namespace utils
{
namespace geo
{
namespace
{
inline olp::geo::GeoCoordinates ToOlp( const map_service::GeoCoordinates& in )
{
    return olp::geo::GeoCoordinates::FromDegrees(
        in.get< 1 >( ),
        in.get< 0 >( ) );
}

inline olp::geo::GeoRectangle ToOlp( const map_service::GeoRectangle& in )
{
    return olp::geo::GeoRectangle(
        ToOlp( in.min_corner( ) ),
        ToOlp( in.max_corner( ) ) );
}


} // unnamed namespace


std::vector< PartitionId >
ToTileIds( const std::vector< GeoCoordinates >& polyline, double corridor_width_in_meters, std::uint32_t zoom_level )
{
    // TODO: use corridor_width_in_meters
    using namespace olp::geo;

    HalfQuadTreeIdentityTilingScheme tiling_scheme;
    std::unordered_set< PartitionId > ids;
    for ( const auto& geo_point : polyline )
    {
        ids.insert( TileKeyUtils::GeoCoordinatesToTileKey( tiling_scheme, ToOlp( geo_point ), zoom_level ).ToHereTile( ) );
    }
    return std::vector< PartitionId >( ids.cbegin( ), ids.cend( ) );
}

std::vector< PartitionId >
ToTileIds( const GeoRectangle& box, std::uint32_t zoom_level )
{
    using namespace olp::geo;
    const auto keys = TileKeyUtils::GeoRectangleToTileKeys( HalfQuadTreeIdentityTilingScheme( ), ToOlp( box ), zoom_level );

    std::vector< PartitionId > result;
    result.reserve( keys.size( ) );

    auto to_here_tile = []( const TileKey& key )
    { return key.ToHereTile( ); };
    std::transform( keys.cbegin( ), keys.cend( ), std::back_inserter( result ), to_here_tile );
    return result;
}

} // namespace geo
} // namespace utils
} // namespace map_service
