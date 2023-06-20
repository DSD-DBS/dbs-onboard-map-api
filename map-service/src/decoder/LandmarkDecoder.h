/*
 * SPDX-FileCopyrightText: Copyright DB Netz AG
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#include <decoder/BaseDecoder.h>
#include <map-service/model/Landmarks.h>
#include <memory>

namespace map_service
{

class MapServiceConfig;

namespace decoder
{
class LayerDecoder;

class LandmarksDecoder : public BaseDecoder< std::vector< model::Landmark::Ptr > >
{
public:
    LandmarksDecoder( const MapServiceConfig& config );

    ~LandmarksDecoder( );

    std::vector< model::Landmark::Ptr > ConstructLayer( const std::vector< PartitionId >& tiles ) const override;

private:
    std::unique_ptr< LayerDecoder > decoder_;
};

} // namespace decoder
} // namespace map_service
