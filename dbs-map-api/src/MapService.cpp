/*
 * SPDX-FileCopyrightText: Copyright DB Netz AG
 * SPDX-License-Identifier: Apache-2.0
 */

#include <dbs-map-api/MapService.h>
#include "MapServiceImpl.h"

namespace map_service
{

MapService::MapService( const MapServiceConfig& config )
: impl_( std::make_shared< MapServiceImpl >( config ) )
{
}

MapService::~MapService( )
{
}

Version MapService::GetLocalMapVersion( ) const
{
    return impl_->GetLocalMapVersion( );
}

Version MapService::GetCloudMapVersion( ) const
{
    return impl_->GetCloudMapVersion( );
}

model::ConsolidatedLayers::Ptr MapService::GetLayersForRectangle( const GeoRectangle& rectangle ) const
{
    return impl_->GetLayersForRectangle( rectangle );
}

model::ConsolidatedLayers::Ptr MapService::GetLayersForTiles( const std::vector< PartitionId >& tile_ids ) const
{
    return impl_->GetLayersForTiles( tile_ids );
}
model::ConsolidatedLayers::Ptr MapService::GetLayersForCorridor( const std::vector< GeoCoordinates >& polyline, double corridor_width_in_meters ) const
{
    return impl_->GetLayersForCorridor( polyline, corridor_width_in_meters );
}

model::RcaTopology::Ptr MapService::GetRcaTopology( const std::vector< PartitionId >& tile_ids ) const
{
    return impl_->GetRcaTopology( tile_ids );
}

std::vector< model::Landmark::Ptr > MapService::GetLandmarks( const std::vector< PartitionId >& tile_ids ) const
{
    return impl_->GetLandmarks( tile_ids );
}

std::vector< model::Zone::Ptr > MapService::GetZones( const std::vector< PartitionId >& tile_ids ) const
{
    return impl_->GetZones( tile_ids );
}

void
MapService::CleanLocalCache( ) const
{
    return impl_->CleanLocalCache( );
}

void
MapService::UpdateLocalMap( const Version& version ) const
{
    return impl_->UpdateLocalMap( version );
}

std::vector< MapService::Error >
MapService::UpdateMap( const Version& version ) const noexcept
{
    return impl_->UpdateMap( version );
}
} // namespace map_service
