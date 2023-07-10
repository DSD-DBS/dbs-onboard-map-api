/*
 * SPDX-FileCopyrightText: Copyright DB Netz AG
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#include <rapidjson/document.h>
#include <memory>

namespace dbs_map
{
namespace download
{
struct ClientSettings;

class HttpClient
{
public:
    HttpClient( const ClientSettings& settings );
    ~HttpClient( );

    void GetUri( const std::string& url, std::ostream& out ) const;
    void GetUri( const std::string& url, rapidjson::Document& out ) const;

private:
    std::shared_ptr< ClientSettings > client_settings_;
};
} // namespace download
} // namespace dbs_map
