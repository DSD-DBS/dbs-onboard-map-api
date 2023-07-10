/*
 * SPDX-FileCopyrightText: Copyright DB Netz AG
 * SPDX-License-Identifier: Apache-2.0
 */

#include <dbs-map-api/MapService.h>
#include <dbs-map-api/MapServiceConfig.h>
#include <dbs-map-api/utils/MapFileSystem.h>
#include <dbs-map-api/download/LayerClient.h>
#include <dbs-map-api/download/CatalogClient.h>
#include <dbs-map-api/download/HttpRuntimeError.h>

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <curlpp/Exception.hpp>

#include <filesystem>

namespace
{
using namespace map_service;
using namespace map_service::model;
using namespace map_service::download;

namespace fs = std::filesystem;

MapServiceConfig GetTestConfig( )
{
    auto result = GetDefaultConfig( );
    result.map_local_path_ = std::filesystem::current_path( ) / "build" / "map_update_folder";
    result.http_client_settings_ = download::GetDBClientForCISettings( );

    return result;
}

class MapUpdateTest : public ::testing::Test
{
public:
    virtual void SetUp( )
    {
        MapService( GetTestConfig( ) ).CleanLocalCache( );
    }

    virtual void TearDown( )
    {
        MapService( GetTestConfig( ) ).CleanLocalCache( );
    }
};

void CheckLayer( const MapServiceConfig& cfg, const std::string& layer_name, const Version& version )
{
    LayerClient client( cfg.catalog_, layer_name, version, cfg.http_client_settings_ );
    utils::MapFileSystem map_fs( cfg.map_local_path_, cfg.catalog_ );

    const auto response = client.GetAllPartitionsMetadata( );
    for ( const auto& meta : response )
    {
        ASSERT_TRUE( fs::exists( map_fs.GetCurrentMapPartitionPath( layer_name, meta.id_ ) ) );
        ASSERT_TRUE( fs::exists( map_fs.GetPartitionPath( version, layer_name, meta.id_ ) ) );
        // TODO: hceck file size and checksum
    }
}

} // unnamed namespace

TEST( MapUpdateTest, DISABLED_HostDoesNotExist )
{
    // Arrange
    auto cfg = GetTestConfig( );
    cfg.http_client_settings_.host_ = "does.not.exist.digitalregister.az4db-iat.comp.db.de";

    // Act / Assert
    EXPECT_THROW( { MapService( cfg ).UpdateLocalMap( 10 ); }, curlpp::LibcurlRuntimeError );
}

TEST( MapUpdateTest, DISABLED_CatalogDoesNotExist )
{
    // Arrange
    auto cfg = GetTestConfig( );
    cfg.catalog_ = "does-not-exists";

    // Act / Assert
    EXPECT_THROW( { MapService( cfg ).UpdateLocalMap( 10 ); }, download::HttpRuntimeError );
}

TEST( MapUpdateTest, DISABLED_HostDoesNotExistNoThrow )
{
    // Arrange
    auto cfg = GetTestConfig( );
    cfg.http_client_settings_.host_ = "does.not.exist.digitalregister.az4db-iat.comp.db.de";

    // Act
    const auto errors = MapService( cfg ).UpdateMap( 10 );

    // Assert
    ASSERT_EQ( errors.size( ), 1 );
    ASSERT_EQ( errors[ 0 ].error_code_, ErrorCode::CurlError );
}

TEST( MapUpdateTest, DISABLED_CatalogDoesNotExistNoThrow )
{
    // Arrange
    auto cfg = GetTestConfig( );
    cfg.catalog_ = "does-not-exists";

    // Act
    const auto errors = MapService( cfg ).UpdateMap( 10 );

    // Assert
    ASSERT_EQ( errors.size( ), 1 );
    ASSERT_EQ( errors[ 0 ].error_code_, ErrorCode::HttpError );
}

TEST( MapUpdateTest, DISABLED_CleanLocalCache )
{
    // Arrange
    const auto cfg = GetTestConfig( );
    MapService service( cfg );
    CatalogClient catalog_client( cfg.catalog_, cfg.http_client_settings_ );

    service.UpdateLocalMap( catalog_client.GetLatestVersion( ) );

    utils::MapFileSystem map_fs( cfg.map_local_path_, cfg.catalog_ );
    ASSERT_FALSE( fs::is_empty( map_fs.GetCatalogPath( ) ) );

    // Act
    service.CleanLocalCache( );

    // Asserts
    ASSERT_TRUE( fs::exists( cfg.map_local_path_ ) );
}

TEST( MapUpdateTest, DISABLED_UpdateMapFromScratch )
{
    // Arrange
    const auto cfg = GetTestConfig( );
    utils::MapFileSystem map_fs( cfg.map_local_path_, cfg.catalog_ );
    CatalogClient catalog_client( cfg.catalog_, cfg.http_client_settings_ );
    const auto version = catalog_client.GetLatestVersion( );

    MapService service( cfg );
    const auto layer_list = {
        cfg.layer_rca_topoloy_,
        cfg.layer_centerline_,
        cfg.layer_landmarks_,
        cfg.layer_zones_,
    };

    // Act
    service.UpdateLocalMap( version );

    // Asserts
    const auto catalog_path = map_fs.GetCatalogPath( );
    ASSERT_TRUE( fs::exists( map_fs.GetCatalogPath( ) ) );
    ASSERT_TRUE( fs::exists( map_fs.GetCurrentMapPath( ) ) );
    ASSERT_TRUE( fs::exists( map_fs.GetCatalogVersionPath( version ) ) );

    ASSERT_FALSE( fs::is_empty( map_fs.GetCatalogPath( ) ) );
    ASSERT_FALSE( fs::is_empty( map_fs.GetCurrentMapPath( ) ) );
    ASSERT_FALSE( fs::is_empty( map_fs.GetCatalogVersionPath( version ) ) );

    for ( const auto& layer : layer_list )
    {
        CheckLayer( cfg, layer, version );
    }
}

TEST( MapUpdateTest, DISABLED_UpdateMapIncrementally )
{
    // Arrange
    const auto cfg = GetTestConfig( );
    utils::MapFileSystem map_fs( cfg.map_local_path_, cfg.catalog_ );

    MapService service( cfg );
    CatalogClient catalog_client( cfg.catalog_, cfg.http_client_settings_ );
    const auto version_to = catalog_client.GetLatestVersion( );
    const auto version_from = version_to - 1;

    const auto layer_list = {
        cfg.layer_rca_topoloy_,
        cfg.layer_centerline_,
        cfg.layer_landmarks_,
        cfg.layer_zones_,
    };
    service.UpdateLocalMap( version_from );
    ASSERT_TRUE( fs::exists( map_fs.GetCatalogVersionPath( version_from ) ) );

    // Act
    service.UpdateLocalMap( version_to );

    // Asserts
    const auto catalog_path = map_fs.GetCatalogPath( );
    ASSERT_TRUE( fs::exists( map_fs.GetCatalogPath( ) ) );
    ASSERT_TRUE( fs::exists( map_fs.GetCurrentMapPath( ) ) );
    ASSERT_TRUE( fs::exists( map_fs.GetCatalogVersionPath( version_to ) ) );
    ASSERT_FALSE( fs::exists( map_fs.GetCatalogVersionPath( version_from ) ) );

    ASSERT_FALSE( fs::is_empty( map_fs.GetCatalogPath( ) ) );
    ASSERT_FALSE( fs::is_empty( map_fs.GetCurrentMapPath( ) ) );
    ASSERT_FALSE( fs::is_empty( map_fs.GetCatalogVersionPath( version_to ) ) );

    for ( const auto& layer : layer_list )
    {
        CheckLayer( cfg, layer, version_to );
    }
}