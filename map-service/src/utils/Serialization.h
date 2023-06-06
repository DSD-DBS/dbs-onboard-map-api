#pragma once
#include <map-service/download/Types.h>
#include <utils/HttpClient.h>

#include <rapidjson/document.h>

namespace map_service
{
namespace utils
{
namespace serialization
{

template < class T >
T DeserialiseObject( const rapidjson::Value& doc );

template < class T >
download::Response< T > GetObjectFromUri( const utils::HttpClient& http_client, const std::string& url )
{
    using namespace download;

    rapidjson::Document doc;

    Response< T > response = { http_client.GetUri( url, doc ) };
    if ( response.error_.error_code_ == ErrorCode::Success )
    {
        response.content_ = DeserialiseObject< T >( doc );
    }
    return response;
}


} // namespace serialization
} // namespace utils
} // namespace map_service