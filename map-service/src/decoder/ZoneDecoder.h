/*
 * SPDX-FileCopyrightText: Copyright DB Netz AG
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#include <decoder/BaseDecoder.h>
#include <map-service/model/Zone.h>
#include <vector>

namespace map_service
{
struct MapServiceConfig;

namespace decoder
{
class LayerDecoder;

class ZoneDecoder : public BaseDecoder< std::vector< model::Zone::Ptr > >
{
public:
    ZoneDecoder( const MapServiceConfig& config );

    ~ZoneDecoder( );

    std::vector< model::Zone::Ptr > ConstructLayer( const std::vector< PartitionId >& tile_ids ) const override;

private:
    std::unique_ptr< LayerDecoder > decoder_;
};

} // namespace decoder
} // namespace map_service
