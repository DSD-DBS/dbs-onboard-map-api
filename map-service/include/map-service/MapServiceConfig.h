/*
 * SPDX-FileCopyrightText: Copyright DB Netz AG
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#include <map-service/CommonTypes.h>
#include <map-service/download/ClientSettings.h>

namespace map_service
{
struct MapServiceConfig
{
    std::string map_local_path_;

    // Technically, map layers could have differet zoom level.
    // better to read zoom laevel from OLP layer metadata.
    std::uint32_t zoom_level_ = 14;

    // DB catalog + layers
    // std::string catalog_ = "sensors4rail2";
    std::string catalog_ = "validate.s4r2.oss.4";
    std::string layer_rca_topoloy_ = "rca-topology";
    std::string layer_centerline_ = "rca-centerline";
    std::string layer_landmarks_ = "landmarks";
    std::string layer_zones_ = "risk-assessment-zones";

    download::ClientSettings http_client_settings_ = download::GetDBClientSettings( );
};

MapServiceConfig GetDefaultConfig( );

} // namespace map_service
