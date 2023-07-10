/*
 * SPDX-FileCopyrightText: Copyright DB Netz AG
 * SPDX-License-Identifier: Apache-2.0
 */

#include <dbs-map-api/download/LayerClient.h>
#include <dbs-map-api/download/CatalogClient.h>
#include <dbs-map-api/download/HttpRuntimeError.h>
#include <dbs-map-api/MapServiceConfig.h>

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <curlpp/Exception.hpp>

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

Version GetLatestVersion( )
{
    const auto cfg = GetTestConfig( );
    return CatalogClient( cfg.catalog_, cfg.http_client_settings_ ).GetLatestVersion( );
}


} // unnamed namespace

TEST( LayerClient, DISABLED_HostDoesNotExist )
{
    // Arrange
    auto cfg = GetTestConfig( );
    cfg.http_client_settings_.host_ = "does.not.exist.digitalregister.az4db-iat.comp.db.de";
    LayerClient client( cfg.catalog_, "rca-topology", 1, cfg.http_client_settings_ );

    // Act / Assert
    EXPECT_THROW( { client.GetMetadata( ); }, curlpp::LibcurlRuntimeError );
}

TEST( LayerClient, DISABLED_LayerDoesNotExist )
{
    // Arrange
    auto cfg = GetTestConfig( );
    LayerClient client( cfg.catalog_, "does-not-exists", 1, cfg.http_client_settings_ );

    // Act / Assert
    EXPECT_THROW( { client.GetMetadata( ); }, download::HttpRuntimeError );
}

TEST( LayerClient, DISABLED_CatalogDoesNotExist )
{
    // Arrange
    auto cfg = GetTestConfig( );
    LayerClient client( "does-not-exists", "rca-topology", 1, cfg.http_client_settings_ );

    // Act / Assert
    EXPECT_THROW( { client.GetMetadata( ); }, download::HttpRuntimeError );
}


TEST( LayerClient, DISABLED_GetMetadata )
{
    // Arrange
    const auto cfg = GetTestConfig( );
    LayerClient client( cfg.catalog_, "rca-topology", GetLatestVersion( ), cfg.http_client_settings_ );

    // Act
    auto response = client.GetMetadata( );

    // Assert
    ASSERT_EQ( response.catalogId_, cfg.catalog_ );
    ASSERT_EQ( response.layerId_, "rca-topology" );
    ASSERT_FALSE( response.zoom_levels_.empty( ) );
    ASSERT_EQ( response.zoom_levels_.front( ), 12 );
    ASSERT_EQ( response.schema_name_, "org.db.schema.rca.topology.v2.BaseLayerTile" );
    // ASSERT_EQ( response.content_.partitioning_; HereTiling
    ASSERT_EQ( response.content_type_, "application/x-protobuf" );
    ASSERT_EQ( response.content_encoding_, "none" );
    // ASSERT_EQ( response.content_.modificationDate
    ASSERT_NE( response.description_, "" );
}

TEST( LayerClient, DISABLED_GetPartitionMetadata )
{
    // Arrange
    const auto cfg = GetTestConfig( );
    LayerClient client( cfg.catalog_, "rca-topology", GetLatestVersion( ), cfg.http_client_settings_ );

    // Act
    auto response = client.GetPartitionMetadata( "23608580" );

    // Assert
    ASSERT_EQ( response.catalog_id_, cfg.catalog_ );
    ASSERT_GT( response.catalog_version_, 0 );
    ASSERT_EQ( response.id_, "23608580" );
    ASSERT_GT( response.data_size_, 0 );
    ASSERT_NE( response.check_sum_, 0 );
    ASSERT_GT( response.modification_date_, 1648824006 );
    ASSERT_NE( response.data_handle_, "" );
}

TEST( LayerClient, DISABLED_GetDataHandle )
{
    // Arrange
    const auto cfg = GetTestConfig( );
    LayerClient client( cfg.catalog_, "rca-topology", GetLatestVersion( ), cfg.http_client_settings_ );

    const auto response_meta = client.GetPartitionMetadata( "23608580" );

    std::stringstream ss;

    // Act
    client.GetDataHandle( response_meta.data_handle_, ss );

    // Assert
    ASSERT_FALSE( ss.str( ).empty( ) );
}

TEST( LayerClient, DISABLED_GetDifference )
{
    // Arrange
    const auto latest_version = GetLatestVersion( );
    const auto cfg = GetTestConfig( );
    LayerClient client( cfg.catalog_, "rca-topology", latest_version, cfg.http_client_settings_ );

    // Act
    const auto response = client.GetDifference( 1 );

    // Assert
    ASSERT_FALSE( response.empty( ) );
}

TEST( LayerClient, DISABLED_GetAllPartitionsMetadata )
{
    // Arrange
    const auto cfg = GetTestConfig( );
    LayerClient client( cfg.catalog_, "rca-topology", GetLatestVersion( ), cfg.http_client_settings_ );

    // Act
    const auto response = client.GetAllPartitionsMetadata( );

    // Assert
    ASSERT_FALSE( response.empty( ) );
}

TEST( LayerClient, DISABLED_CatalogVeresion )
{
    // Arrange
    const auto catalog_version = GetLatestVersion( );
    const auto cfg = GetTestConfig( );
    LayerClient client( cfg.catalog_, "rca-topology", catalog_version, cfg.http_client_settings_ );

    // Act
    const auto response = client.GetPartitionMetadata( "23608580" );

    // Assert
    ASSERT_GT( response.catalog_version_, 0 );
    ASSERT_LE( response.catalog_version_, catalog_version );
}