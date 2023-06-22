/*
 * SPDX-FileCopyrightText: Copyright DB Netz AG
 * SPDX-License-Identifier: Apache-2.0
 */

#include <map-service/MapService.h>
#include <map-service/MapServiceConfig.h>
#include <map-service/utils/MapFileSystem.h>
#include <map-service/download/LayerClient.h>
#include <map-service/download/CatalogClient.h>

#include <gtest/gtest.h>
#include <gmock/gmock.h>
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
    for ( const auto& meta : response.content_ )
    {
        ASSERT_TRUE( fs::exists( map_fs.GetCurrentMapPartitionPath( layer_name, meta.id_ ) ) );
        ASSERT_TRUE( fs::exists( map_fs.GetPartitionPath( version, layer_name, meta.id_ ) ) );
        // TODO: hceck file size and checksum
    }
}

} // unnamed namespace

TEST_F( MapUpdateTest, DISABLED_CleanLocalCache )
{
    // Arrange
    const auto cfg = GetTestConfig( );
    MapService service( cfg );
    CatalogClient catalog_client( cfg.catalog_, cfg.http_client_settings_ );

    service.UpdateMap( catalog_client.GetLatestVersion( ) );

    utils::MapFileSystem map_fs( cfg.map_local_path_, cfg.catalog_ );
    ASSERT_FALSE( fs::is_empty( map_fs.GetCatalogPath( ) ) );

    // Act
    service.CleanLocalCache( );

    // Asserts
    ASSERT_TRUE( fs::exists( cfg.map_local_path_ ) );
}

TEST_F( MapUpdateTest, DISABLED_UpdateMapFromScratch )
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
    const auto result = service.UpdateMap( version );

    ASSERT_TRUE( result.empty( ) );

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

TEST_F( MapUpdateTest, DISABLED_UpdateMapIncrementally )
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
    ASSERT_TRUE( service.UpdateMap( version_from ).empty( ) );
    ASSERT_TRUE( fs::exists( map_fs.GetCatalogVersionPath( version_from ) ) );

    // Act
    ASSERT_TRUE( service.UpdateMap( version_to ).empty( ) );

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