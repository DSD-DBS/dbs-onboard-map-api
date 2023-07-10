/*
 * SPDX-FileCopyrightText: Copyright DB Netz AG
 * SPDX-License-Identifier: Apache-2.0
 */

#include <download/CatalogClientImpl.h>
#include <download/HttpClient.h>
#include <dbs-map-api/download/ClientSettings.h>

#include <boost/format.hpp>


namespace map_service
{

namespace utils
{
namespace serialization
{
using namespace map_service::download::model;


Catalog
DeserialiseCatalog( const rapidjson::Value& doc )
{
    Catalog result;

    result.id_ = doc[ "catalogId" ].GetString( );
    result.version_ = doc[ "catalogVersion" ].GetUint64( );
    result.latest_version_ = doc[ "latestVersion" ].GetUint64( );
    result.modification_date_ = doc[ "modificationDate" ].GetUint64( );
    result.description_ = doc[ "description" ].GetString( );

    for ( const auto& layer_name : doc[ "layerIds" ].GetArray( ) )
    {
        result.layers_.emplace_back( layer_name.GetString( ) );
    }
    return result;
}

} // namespace serialization
} // namespace utils

namespace download
{
using namespace utils::serialization;

CatalogClientImpl::CatalogClientImpl( const std::string& catalog_name, const ClientSettings& settings )
: catalog_name_( catalog_name ),
  settings_( std::make_shared< ClientSettings >( settings ) ),
  http_client_( std::make_shared< download::HttpClient >( settings ) )
{
}

CatalogClientImpl::~CatalogClientImpl( ) = default;

model::Catalog
CatalogClientImpl::GetMetadata( Version version ) const
{
    const auto url = boost::format( "https://%1%/catalogs/%2%?catalogVersion=%3%" ) % settings_->host_ % catalog_name_ % version;

    rapidjson::Document doc;
    http_client_->GetUri( url.str( ), doc );

    return DeserialiseCatalog( doc );
}

map_service::Version
CatalogClientImpl::GetLatestVersion( ) const
{
    const auto url = boost::format( "https://%1%/catalogs/%2%" ) % settings_->host_ % catalog_name_;

    rapidjson::Document doc;
    http_client_->GetUri( url.str( ), doc );

    return DeserialiseCatalog( doc ).latest_version_;

}
} // namespace download
} // namespace map_service
