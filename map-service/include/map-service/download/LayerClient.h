/*
 * SPDX-FileCopyrightText: Copyright DB Netz AG
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#include <map-service/download/model/Layer.h>
#include <map-service/download/model/Partition.h>

#include <memory>

namespace map_service
{
namespace download
{

class LayerClientImpl;
struct ClientSettings;

class LayerClient
{
public:
    LayerClient( const std::string& catalog_name, const std::string& layer_name,
        Version catalog_version, const ClientSettings& settings );

    model::Layer GetMetadata( ) const;
    model::Partition GetPartitionMetadata( const PartitionId& id ) const;
    void WriteData( const DataHandle& data_handle, std::ostream& out ) const;
    std::vector< model::Partition > GetDifference( const Version& from_version ) const;
    std::vector< model::Partition > GetAllPartitionsMetadata( ) const;

private:
    std::shared_ptr< LayerClientImpl > impl_;
};

} // namespace download
} // namespace map_service
