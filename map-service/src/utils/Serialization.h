/*
 * SPDX-FileCopyrightText: Copyright DB Netz AG
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#include <map-service/download/Types.h>
#include <download/HttpClient.h>

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
download::Response< T > GetObjectFromUri( const download::HttpClient& http_client, const std::string& url )
{
    rapidjson::Document doc;
    http_client.GetUri( url, doc );

    return download::Response< T >{ DeserialiseObject< T >( doc ) };
}


} // namespace serialization
} // namespace utils
} // namespace map_service
