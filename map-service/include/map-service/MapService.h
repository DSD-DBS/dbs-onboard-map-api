/*
 * SPDX-FileCopyrightText: Copyright DB Netz AG
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#include <map-service/model/ConsolidatedLayers.h>
#include <map-service/download/Response.h>

namespace map_service
{
class MapServiceConfig;
class MapServiceImpl;

namespace download
{

// TODO: This errors needs to be removed.
// Map-Service project uses exceptions everywhere, not error codes.
// Error code are kept for backward compatibiliy with Rail Horizon.
enum ErrorCode
{
    Unknown = 0,
    Success,
    CurlError, // Check CURLCode for more details
    HttpError, // Check HttpCode for more details
};

struct Error
{
    ErrorCode error_code_ = ErrorCode::Success;
    std::string msg_;
    CURLcode curl_code_ = CURLE_OK;
    long http_code_ = 200;
};
} // namespace download

/**
 * @brief Service for providing map data. It supports Request / Response pattern.
 * Map data is served in WSG84 coordinaes and could be requested by varoius geo areas:
 * geo circle, geo rectangle, corridor or by tile ids.
 */
class MapService
{
public:
    using Error = map_service::download::Error;

    MapService( const MapServiceConfig& config );
    ~MapService( );

    /**
     * @brief Returns current local map version
     *
     * @return Local map version
     */
    Version GetLocalMapVersion( ) const;

    /**
     * @brief Returns current map version at cloud
     *
     * @return Cloud map version
     */
    Version GetCloudMapVersion( ) const;

    /**
     * @brief Get the consolidated layers data for specified rectangle.
     *
     * @param rectange
     * @return model::ConsolidatedLayers::Ptr
     */
    model::ConsolidatedLayers::Ptr GetLayersForRectangle( const GeoRectangle& rectange ) const;

    /**
     * @brief  Get the consolidated layers data for specified list of partitions.
     *
     * @param tile_ids in HERE OLP format.
     * @return model::ConsolidatedLayers::Ptr
     */
    model::ConsolidatedLayers::Ptr GetLayersForTiles( const std::vector< PartitionId >& tile_ids ) const;

    /**
     * @brief  Get the consolidated layers data for specified corridor.
     *
     * @param polyline center line of corridor
     * @param corridor_width_in_meters
     * @return model::ConsolidatedLayers::Ptr
     */
    model::ConsolidatedLayers::Ptr GetLayersForCorridor( const std::vector< GeoCoordinates >& polyline, double corridor_width_in_meters ) const;

    // Low level API for requesting map data per layer for given partition set.
    /**
     * @brief Get the topology in RCA format. Track object is enreached by geometry.
     *
     * @param tile_ids list of tile IDs for which retrun map data.
     * @return Collection of TrackEdge and Nodes ptrs
     */
    model::RcaTopology::Ptr GetRcaTopology( const std::vector< PartitionId >& tile_ids ) const;

    /**
     * @brief Get the Landmarks layer data: poles, walls, platforms and etc.
     *
     * @@param tile_ids list of tile IDs for which retrun map data.
     * @return collection of landmarks
     */
    std::vector< model::Landmark::Ptr > GetLandmarks( const std::vector< PartitionId >& tile_ids ) const;

    /**
     * @brief Get the Zones layer data
     *
     * @param tile_ids list of tile IDs for which retrun map data.
     * @return collection of zones
     */
    std::vector< model::Zone::Ptr > GetZones( const std::vector< PartitionId >& tile_ids ) const;

    /**
     * @brief Delete all locally stored map data.
     *
     */
    void CleanLocalCache( ) const;

    /**
     * @brief Incrementaly update current local map to the specified version
     *
     * @param version to which map will be updated
     */
    void UpdateLocalMap( const Version& version ) const;

    /**
     * @brief Incrementaly update current local map to the specified version
     *
     * @param version to which map will be updated
     * @return std::vector< Error >  List of errors: tile not downloaded, layer metadata is not retreived and etc.
     */
    std::vector< Error > UpdateMap( const Version& version ) const noexcept;

private:
    std::shared_ptr< MapServiceImpl > impl_;
};
} // namespace map_service