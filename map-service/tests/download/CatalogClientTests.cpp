/*
 * SPDX-FileCopyrightText: Copyright DB Netz AG
 * SPDX-License-Identifier: Apache-2.0
 */

#include <map-service/download/CatalogClient.h>
#include <map-service/download/HttpRuntimeError.h>
#include <map-service/MapServiceConfig.h>

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
    // const auto& error = response.error_;
    // ASSERT_EQ( error.error_code_, ErrorCode::Success );
    // ASSERT_EQ( error.curl_code_, CURLE_OK );

    const auto& content = response.content_;

    ASSERT_EQ( content.id_, cfg.catalog_ );
    // TODO: Currently, catalog.version_ always contains latest_version_
    // Catalog.version_ should be the version form request.
    ASSERT_GT( content.version_, 0 );
    ASSERT_GT( content.latest_version_, 0 );
    ASSERT_GT( content.modification_date_, 0 );
    ASSERT_FALSE( content.description_.empty( ) );
    ASSERT_FALSE( content.layers_.empty( ) );

    std::unordered_set< std::string > layers = {
        "rca-topology",
        "rca-topology-geometry",
        "risk-assessment-zones",
        "landmarks",
        "rca-centerline",
    };

    for ( const auto& layer : layers )
    {
        ASSERT_NE( std::find( content.layers_.cbegin( ), content.layers_.cend( ), layer ), content.layers_.cend( ) );
    }
}
