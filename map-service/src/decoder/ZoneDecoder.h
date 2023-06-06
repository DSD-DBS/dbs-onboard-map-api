#pragma once
#include <decoder/BaseDecoder.h>
#include <map-service/model/Zone.h>
#include <vector>

namespace map_service
{
class MapServiceConfig;

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