/*
 * SPDX-FileCopyrightText: Copyright DB Netz AG
 * SPDX-License-Identifier: Apache-2.0
 */

#include <map-service/download/LayerClient.h>
#include <download/LayerClientImpl.h>

namespace map_service
{
namespace download
{

LayerClient::LayerClient( const std::string& catalog_name, const std::string& layer_name,
    Version catalog_version, const ClientSettings& settings )
: impl_( std::make_shared< LayerClientImpl >( catalog_name, layer_name, catalog_version, settings ) )
{
}

LayerResponse
LayerClient::GetMetadata( ) const
{
    return impl_->GetMetadata( );
}

PartitionResponse
LayerClient::GetPartitionMetadata( const PartitionId& id ) const
{
    return impl_->GetPartitionMetadata( id );
}

void
LayerClient::WriteData( const DataHandle& data_handle, std::ostream& out ) const
{
    impl_->WriteData( data_handle, out );
}

PartitionListResponse
LayerClient::GetDifference( const map_service::Version& from_version ) const
{
    return impl_->GetDifference( from_version );
}

PartitionListResponse
LayerClient::GetAllPartitionsMetadata( ) const
{
    return impl_->GetAllPartitionsMetadata( );
}
} // namespace download
} // namespace map_service
