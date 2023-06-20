/*
 * SPDX-FileCopyrightText: Copyright DB Netz AG
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#include <map-service/download/Types.h>

namespace map_service
{
namespace utils
{
class HttpClient;

} // namespace utils

namespace download
{
struct ClientSettings;

class CatalogClientImpl
{
public:
    CatalogClientImpl( const std::string& catalog_name, const ClientSettings& settigns );
    ~CatalogClientImpl( );

    CatalogResponse GetMetadata( Version version ) const;
    Version GetLatestVersion( ) const;

private:
    std::string catalog_name_;
    std::shared_ptr< ClientSettings > settings_;
    std::shared_ptr< utils::HttpClient > http_client_;
};
} // namespace download
} // namespace map_service
