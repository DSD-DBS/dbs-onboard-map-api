#pragma once
#include <map-service/download/Types.h>

#include <memory>

namespace map_service
{
namespace download
{
class CatalogClientImpl;
class ClientSettings;

class CatalogClient
{
public:
    CatalogClient( const std::string& catalog_name, const ClientSettings& settings );

    CatalogResponse GetMetadata( map_service::Version version ) const;
    Version GetLatestVersion( ) const;

private:
    std::shared_ptr< CatalogClientImpl > impl_;
};
} // namespace download
} // namespace map_service
