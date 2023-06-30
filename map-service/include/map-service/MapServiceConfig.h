/*
 * SPDX-FileCopyrightText: Copyright DB Netz AG
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once
#include <map-service/CommonTypes.h>
#include <map-service/download/ClientSettings.h>

namespace map_service
{
/**
 * @struct MapServiceConfig
 * @brief Configuration structure for the MapService class.
 *
 * This structure defines the configuration parameters used by the MapService class,
 * including the local path for map data, the zoom level, the catalog and layer details,
 * as well as the settings for the HTTP client used for downloading data.
 */
struct MapServiceConfig
{
    /**
     * @brief The local path for map data.
     *
     * This is the path to the local directory where map data will be stored.
     */
    std::string map_local_path_;

    /**
     * @brief The default zoom level for map data.
     *
     * This is the zoom level to be used for map data. This value can be overridden by metadata from individual map layers.
     */
    std::uint32_t zoom_level_ = 12;

    /**
     * @brief The catalog for the database.
     *
     * This is the name of the catalog in the database to be used by the MapService.
     */
    std::string catalog_ = "validate.s4r2.oss.4";

    /**
     * @brief The layer for RCA topology.
     *
     * This is the name of the layer in the catalog that contains RCA topology data.
     */
    std::string layer_rca_topoloy_ = "rca-topology";

    /**
     * @brief The layer for centerline data.
     *
     * This is the name of the layer in the catalog that contains centerline data.
     */
    std::string layer_centerline_ = "rca-centerline";

    /**
     * @brief The layer for landmark data.
     *
     * This is the name of the layer in the catalog that contains landmark data.
     */
    std::string layer_landmarks_ = "landmarks";

    /**
     * @brief The layer for risk assessment zones.
     *
     * This is the name of the layer in the catalog that contains risk assessment zone data.
     */
    std::string layer_zones_ = "risk-assessment-zones";

    /**
     * @brief The settings for the HTTP client used for downloading data.
     *
     * This is the settings object used to configure the HTTP client that is used for downloading data.
     */
    download::ClientSettings http_client_settings_ = download::GetDBClientSettings( );
};

/**
 * @brief Function to get the default MapServiceConfig.
 *
 * This function returns a MapServiceConfig object with default configuration parameters.
 *
 * @return A MapServiceConfig object with default configuration parameters.
 */
MapServiceConfig GetDefaultConfig( );


} // namespace map_service
