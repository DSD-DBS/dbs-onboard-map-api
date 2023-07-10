/*
 * SPDX-FileCopyrightText: Copyright DB Netz AG
 * SPDX-License-Identifier: Apache-2.0
 */

#include <dbs-map-api/download/CatalogClient.h>
#include <download/CatalogClientImpl.h>

namespace dbs_map
{
namespace download
{

CatalogClient::CatalogClient( const std::string& catalog_name, const ClientSettings& settings )
: impl_( std::make_shared< CatalogClientImpl >( catalog_name, settings ) )
{
}

model::Catalog
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
} // namespace dbs_map
