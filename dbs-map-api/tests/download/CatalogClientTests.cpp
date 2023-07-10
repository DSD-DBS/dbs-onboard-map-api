/*
 * SPDX-FileCopyrightText: Copyright DB Netz AG
 * SPDX-License-Identifier: Apache-2.0
 */

#include <dbs-map-api/download/CatalogClient.h>
#include <dbs-map-api/download/HttpRuntimeError.h>
#include <dbs-map-api/MapServiceConfig.h>

#include <curlpp/Exception.hpp>
#include <gtest/gtest.h>

#include <unordered_set>

namespace
{
using namespace map_service;
using namespace map_service::download;

MapServiceConfig GetTestConfig( )
{
    auto result = GetDefaultConfig( );
    result.http_client_settings_ = download::GetDBClientForCISettings( );

    return result;
}

} // unnamed namespace

TEST( CatalogClientTest, DISABLED_HostDoesNotExist )
{
    // Arrange
    auto cfg = GetTestConfig( );
    cfg.http_client_settings_.host_ = "does.not.exist.digitalregister.az4db-iat.comp.db.de";
    CatalogClient client( cfg.catalog_, cfg.http_client_settings_ );

    // Act / Assert
    EXPECT_THROW( { client.GetLatestVersion( ); }, curlpp::LibcurlRuntimeError );
}

TEST( CatalogClientTest, DISABLED_CatalogDoesNotExist )
{
    // Arrange
    auto cfg = GetTestConfig( );
    CatalogClient client( "does-not-exists", cfg.http_client_settings_ );

    // Act / Assert
    EXPECT_THROW( { client.GetLatestVersion( ); }, download::HttpRuntimeError );
}

TEST( CatalogClientTest, DISABLED_GetLatestVersion )
{
    // Arrange
    const auto cfg = GetTestConfig( );
    CatalogClient client( cfg.catalog_, cfg.http_client_settings_ );

    // Act / Assert
    ASSERT_GT( client.GetLatestVersion( ), 0 );
}

TEST( CatalogClientTest, DISABLED_GetMetadata )
{
    // Arrange
    Version catalog_version = 3;
    const auto cfg = GetTestConfig( );
    CatalogClient client( cfg.catalog_, cfg.http_client_settings_ );

    // Act
    auto response = client.GetMetadata( catalog_version );

    // Assert
    ASSERT_EQ( response.id_, cfg.catalog_ );
    // TODO: Currently, catalog.version_ always contains latest_version_
    // Catalog.version_ should be the version form request.
    ASSERT_GT( response.version_, 0 );
    ASSERT_GT( response.latest_version_, 0 );
    ASSERT_GT( response.modification_date_, 0 );
    ASSERT_FALSE( response.description_.empty( ) );
    ASSERT_FALSE( response.layers_.empty( ) );

    std::unordered_set< std::string > layers = {
        "rca-topology",
        "rca-topology-geometry",
        "risk-assessment-zones",
        "landmarks",
        "rca-centerline",
    };

    for ( const auto& layer : layers )
    {
        ASSERT_NE( std::find( response.layers_.cbegin( ), response.layers_.cend( ), layer ), response.layers_.cend( ) );
    }
}
