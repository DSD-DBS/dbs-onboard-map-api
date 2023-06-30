/*
 * SPDX-FileCopyrightText: Copyright DB Netz AG
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#include <map-service/download/model/Layer.h>
#include <map-service/download/model/Partition.h>

namespace map_service
{
namespace download
{
struct ClientSettings;
class HttpClient;

class LayerClientImpl
{
public:
    LayerClientImpl( std::string catalog_namae, std::string layer_name,
        Version catalog_version, const ClientSettings& settings );

    model::Layer GetMetadata( ) const;
    model::Partition GetPartitionMetadata( const PartitionId& id ) const;
    void WriteData( const DataHandle& data_handle, std::ostream& out ) const;
    std::vector< model::Partition > GetDifference( const Version& from_version ) const;
    std::vector< model::Partition > GetAllPartitionsMetadata( ) const;

private:
    std::string catalog_name_;
    std::string layer_name_;
    Version catalog_version_;
    std::shared_ptr< ClientSettings > settings_;
    std::shared_ptr< download::HttpClient > http_client_;
};

} // namespace download
} // namespace map_service
