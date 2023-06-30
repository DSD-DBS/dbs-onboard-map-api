/*
 * SPDX-FileCopyrightText: Copyright DB Netz AG
 * SPDX-License-Identifier: Apache-2.0
 */

#include <download/LayerClientImpl.h>
#include <download/HttpClient.h>
#include <map-service/download/ClientSettings.h>

#include <boost/format.hpp>

namespace map_service
{
namespace utils
{
namespace serialization
{
using namespace map_service::download::model;


Partition DeserialisePartition( const rapidjson::Value& doc )
{
    Partition result;
    result.catalog_id_ = doc[ "catalogId" ].GetString( );
    result.catalog_version_ = doc[ "catalogVersionNr" ].GetUint64( );
    result.id_ = doc[ "blobKey" ].GetString( );

    if ( doc.HasMember( "dataSize" ) )
    {
        result.data_size_ = doc[ "dataSize" ].GetUint64( );
    }

    if ( doc.HasMember( "checksum" ) )
    {
        const auto checksum_str = doc[ "checksum" ].GetString( );
        result.check_sum_ = std::atoi( checksum_str );
    }
    result.modification_date_ = doc[ "modificationDate" ].GetUint64( );
    result.data_handle_ = doc[ "dataHandle" ].GetString( );
    return result;
}


std::vector< Partition >
DeserialisePartitionsList( const rapidjson::Value& doc )
{
    std::vector< Partition > result;

    for ( const auto& metadata : doc[ "blobs" ].GetArray( ) )
    {
        result.emplace_back( DeserialisePartition( metadata ) );
    }
    return result;
}

Layer DeserialiseLayer( const rapidjson::Value& doc )
{
    Layer result;
    result.catalogId_ = doc[ "catalogId" ].GetString( );
    result.layerId_ = doc[ "layerId" ].GetString( );

    result.zoom_levels_.emplace_back( doc[ "zoomLevel" ].GetInt( ) );
    result.schema_name_ = doc[ "schema" ].GetString( );
    // PartitioningType result.content_.partitioning_ = doc["schemaType"].GetString();
    result.content_type_ = doc[ "contentType" ].GetString( );
    result.content_encoding_ = doc[ "contentEncoding" ].GetString( );
    // modificationDate
    result.description_ = doc[ "description" ].GetString( );

    return result;
}

} // namespace serialization
} // namespace utils

namespace download
{
using namespace map_service::utils::serialization;

LayerClientImpl::LayerClientImpl( std::string catalog_name, std::string layer_name,
    Version catalog_version, const ClientSettings& settings )
: catalog_name_( catalog_name ),
  layer_name_( layer_name ),
  catalog_version_( catalog_version ),
  settings_( std::make_shared< ClientSettings >( settings ) ),
  http_client_( std::make_shared< download::HttpClient >( settings ) )
{
}

model::Layer
LayerClientImpl::GetMetadata( ) const
{
    const auto url = boost::format( "https://%1%/catalogs/%2%/layers/%3%?catalogVersion=%4%" ) % settings_->host_ % catalog_name_ % layer_name_ % catalog_version_;

    rapidjson::Document doc;
    http_client_->GetUri( url.str( ), doc );

    return DeserialiseLayer( doc );
}

model::Partition
LayerClientImpl::GetPartitionMetadata( const PartitionId& id ) const
{
    const auto url = boost::format( "https://%1%/catalogs/%2%/layers/%3%/blobs/%4%?catalogVersion=%5%" ) % settings_->host_ % catalog_name_ % layer_name_ % id % catalog_version_;

    rapidjson::Document doc;
    http_client_->GetUri( url.str( ), doc );

    return DeserialisePartition( doc );
}

void LayerClientImpl::GetDataHandle( const DataHandle& data_handle, std::ostream& out ) const
{
    const auto url = boost::format( "https://%1%/blob/catalogs/%2%/layers/%3%/data/%4%?catalogVersion=%5%" ) % settings_->host_ % catalog_name_ % layer_name_ % data_handle % catalog_version_;
    http_client_->GetUri( url.str( ), out );
}

std::vector< model::Partition >
LayerClientImpl::GetDifference( const map_service::Version& from_versoin ) const
{
    const auto url = boost::format( "https://%1%/catalogs/%2%/layers/%3%/changes?fromCatalogVersion=%4%&toCatalogVersion=%5%" ) % settings_->host_ % catalog_name_ % layer_name_ % from_versoin % catalog_version_;

    rapidjson::Document doc;
    http_client_->GetUri( url.str( ), doc );

    return DeserialisePartitionsList( doc );
}

std::vector< model::Partition >
LayerClientImpl::GetAllPartitionsMetadata( ) const
{
    const auto url = boost::format( "https://%1%/catalogs/%2%/layers/%3%/blobs?catalogVersion=%4%" ) % settings_->host_ % catalog_name_ % layer_name_ % catalog_version_;

    rapidjson::Document doc;
    http_client_->GetUri( url.str( ), doc );

    return DeserialisePartitionsList( doc );
}

} // namespace download
} // namespace map_service
