/*
 * SPDX-FileCopyrightText: Copyright DB Netz AG
 * SPDX-License-Identifier: Apache-2.0
 */

#include <map-service/download/LayerClient.h>
#include <map-service/download/CatalogClient.h>
#include <map-service/MapServiceConfig.h>

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


TEST( LayerClient, DISABLED_GetMetadata )
{
    // Arrange
    const auto cfg = GetTestConfig( );
    LayerClient client( cfg.catalog_, "rca-topology", GetLatestVersion( ), cfg.http_client_settings_ );

    // Act
    auto response = client.GetMetadata( );

    // Assert
    const auto& content = response.content_;
    ASSERT_EQ( content.catalogId_, cfg.catalog_ );
    ASSERT_EQ( content.layerId_, "rca-topology" );
    ASSERT_FALSE( content.zoom_levels_.empty( ) );
    ASSERT_EQ( content.zoom_levels_.front( ), 12 );
    ASSERT_EQ( content.schema_name_, "org.db.schema.rca.topology.v2.BaseLayerTile" );
    // ASSERT_EQ( response.content_.partitioning_; HereTiling
    ASSERT_EQ( content.content_type_, "application/x-protobuf" );
    ASSERT_EQ( content.content_encoding_, "none" );
    // ASSERT_EQ( response.content_.modificationDate
    ASSERT_NE( content.description_, "" );
}

TEST( LayerClient, DISABLED_GetPartitionMetadata )
{
    // Arrange
    const auto cfg = GetTestConfig( );
    LayerClient client( cfg.catalog_, "rca-topology", GetLatestVersion( ), cfg.http_client_settings_ );

    // Act
    auto response = client.GetPartitionMetadata( "23608580" );

    // Assert

    const auto& content = response.content_;

    ASSERT_EQ( content.catalog_id_, cfg.catalog_ );
    ASSERT_GT( content.catalog_version_, 0 );
    ASSERT_EQ( content.id_, "23608580" );
    ASSERT_GT( content.data_size_, 0 );
    ASSERT_NE( content.check_sum_, 0 );
    ASSERT_GT( content.modification_date_, 1648824006 );
    ASSERT_NE( content.data_handle_, "" );
}

TEST( LayerClient, DISABLED_WriteData )
{
    // Arrange
    const auto cfg = GetTestConfig( );
    LayerClient client( cfg.catalog_, "rca-topology", GetLatestVersion( ), cfg.http_client_settings_ );

    const auto response_meta = client.GetPartitionMetadata( "23608580" );

    std::stringstream ss;

    // Act
    client.WriteData( response_meta.content_.data_handle_, ss );

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
    const auto& content = response.content_;
    ASSERT_FALSE( content.empty( ) );
}

TEST( LayerClient, DISABLED_GetAllPartitionsMetadata )
{
    // Arrange
    const auto cfg = GetTestConfig( );
    LayerClient client( cfg.catalog_, "rca-topology", GetLatestVersion( ), cfg.http_client_settings_ );

    // Act
    const auto response = client.GetAllPartitionsMetadata( );

    // Assert
    const auto& content = response.content_;
    ASSERT_FALSE( content.empty( ) );
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
    const auto& content = response.content_;
    ASSERT_GT( content.catalog_version_, 0 );
    ASSERT_LE( content.catalog_version_, catalog_version );
}