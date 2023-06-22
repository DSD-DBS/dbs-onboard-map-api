/*
 * SPDX-FileCopyrightText: Copyright DB Netz AG
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#include <map-service/download/Response.h>
#include <rapidjson/document.h>

#include <memory>

namespace map_service
{
namespace download
{
class ClientSettings;
} // namespace download

namespace utils
{
class HttpClient
{
public:
    using ClientSettings = download::ClientSettings;

    HttpClient( const ClientSettings& settings );
    ~HttpClient( );

    void GetUri( const std::string& url, std::ostream& out ) const;
    void GetUri( const std::string& url, rapidjson::Document& out ) const;

private:
    std::shared_ptr< ClientSettings > client_settings_;
};
} // namespace utils
} // namespace map_service
