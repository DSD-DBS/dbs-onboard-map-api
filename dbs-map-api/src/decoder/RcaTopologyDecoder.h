/*
 * SPDX-FileCopyrightText: Copyright DB Netz AG
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#include <decoder/BaseDecoder.h>
#include <dbs-map-api/model/RcaTopology.h>
#include <memory>

namespace map_service
{

struct MapServiceConfig;

namespace decoder
{
class LayerDecoder;

class RcaTopologyDecoder : public BaseDecoder< model::RcaTopology::Ptr >
{
public:
    RcaTopologyDecoder( const MapServiceConfig& config );

    ~RcaTopologyDecoder( );

    model::RcaTopology::Ptr ConstructLayer( const std::vector< PartitionId >& tiles ) const override;

private:
    std::unique_ptr< LayerDecoder > decoder_;
    std::unique_ptr< LayerDecoder > centerline_decoder_;
};

} // namespace decoder
} // namespace map_service
