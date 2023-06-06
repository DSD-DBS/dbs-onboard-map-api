#include "LayerDecoder.h"
#include <map-service/MapServiceConfig.h>

namespace map_service
{
namespace decoder
{

LayerDecoder::LayerDecoder( const MapServiceConfig& config,
    const std::string& catalog,
    const std::string& layer )
: config_( config ), catalog_( catalog ), layer_( layer ), map_fs( config.map_local_path_, catalog )
{
}

} // namespace decoder
} // namespace map_service