/*
 * SPDX-FileCopyrightText: Copyright DB Netz AG
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#include <map-service/download/Types.h>

#include <memory>
#include <future>

namespace map_service
{
namespace download
{

class LayerClientImpl;
class ClientSettings;

class LayerClient
{
public:
    LayerClient( const std::string& catalog_name, const std::string& layer_name,
        Version catalog_version, const ClientSettings& settings );

    LayerResponse GetMetadata( ) const;
    PartitionResponse GetPartitionMetadata( const PartitionId& id ) const;
    Error WriteData( const DataHandle& data_handle, std::ostream& out ) const;
    PartitionListResponse GetDifference( const Version& from_version ) const;
    PartitionListResponse GetAllPartitionsMetadata( ) const;

private:
    std::shared_ptr< LayerClientImpl > impl_;
};

} // namespace download
} // namespace map_service
