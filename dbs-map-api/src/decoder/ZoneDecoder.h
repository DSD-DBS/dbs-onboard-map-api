/*
 * SPDX-FileCopyrightText: Copyright DB Netz AG
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#include <decoder/BaseDecoder.h>
#include <dbs-map-api/model/Zone.h>
#include <vector>

namespace dbs_map
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
} // namespace dbs_map
