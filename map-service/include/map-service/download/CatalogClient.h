/*
 * SPDX-FileCopyrightText: Copyright DB Netz AG
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#include <map-service/download/model/Catalog.h>
#include <memory>

namespace map_service
{
namespace download
{
class CatalogClientImpl;
struct ClientSettings;

class CatalogClient
{
public:
    CatalogClient( const std::string& catalog_name, const ClientSettings& settings );

    model::Catalog GetMetadata( map_service::Version version ) const;
    map_service::Version GetLatestVersion( ) const;

private:
    std::shared_ptr< CatalogClientImpl > impl_;
};
} // namespace download
} // namespace map_service
