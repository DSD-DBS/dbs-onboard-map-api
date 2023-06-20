/*
 * SPDX-FileCopyrightText: Copyright DB Netz AG
 * SPDX-License-Identifier: Apache-2.0
 */

#include <map-service/download/CatalogClient.h>
#include <download/CatalogClientImpl.h>

namespace map_service
{
namespace download
{

CatalogClient::CatalogClient( const std::string& catalog_name, const ClientSettings& settings )
: impl_( std::make_shared< CatalogClientImpl >( catalog_name, settings ) )
{
}

CatalogResponse
CatalogClient::GetMetadata( Version version ) const
{
    return impl_->GetMetadata( version );
}

Version
CatalogClient::GetLatestVersion( ) const
{
    return impl_->GetLatestVersion( );
}
} // namespace download
} // namespace map_service
