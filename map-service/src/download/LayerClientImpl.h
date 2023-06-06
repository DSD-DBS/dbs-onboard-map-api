#pragma once
#include <map-service/download/Types.h>

namespace map_service
{
namespace utils
{
class HttpClient;
} // namespace utils

namespace download
{
class ClientSettings;

class LayerClientImpl
{
public:
    LayerClientImpl( std::string catalog_namae, std::string layer_name,
        Version catalog_version, const ClientSettings& settings );

    LayerResponse GetMetadata( ) const;
    PartitionResponse GetPartitionMetadata( const PartitionId& id ) const;
    Error WriteData( const DataHandle& data_handle, std::ostream& out ) const;
    PartitionListResponse GetDifference( const map_service::Version& from_version ) const;
    PartitionListResponse GetAllPartitionsMetadata( ) const;

private:
    std::string catalog_name_;
    std::string layer_name_;
    Version catalog_version_;
    std::shared_ptr< ClientSettings > settings_;
    std::shared_ptr< utils::HttpClient > http_client_;
};

} // namespace download
} // namespace map_service