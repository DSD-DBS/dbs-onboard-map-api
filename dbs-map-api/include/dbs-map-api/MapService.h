/*
 * SPDX-FileCopyrightText: Copyright DB Netz AG
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#include <dbs-map-api/model/ConsolidatedLayers.h>
#include <curlpp/cURLpp.hpp>

namespace dbs_map
{
struct MapServiceConfig;
class MapServiceImpl;

namespace download
{

/**
 * @enum ErrorCode
 * @brief Enum representing various error codes returned by the MapService::UpdateMap method.
 * This error mechanism is used for backward compatibility with the Rail Horizon project,
 * although MapService is moving towards using exceptions rather than error codes.
 */
enum ErrorCode
{
    Unknown = 0, /**< Unspecified error */
    Success, /**< No error occurred */
    CurlError, /**< An error occurred within the CURL library. Check curl_code_ field for more details */
    HttpError, /**< An HTTP error occurred. Check http_code_ field for more details */
};

/**
 * @struct Error
 * @brief Struct representing an error that occurred during a MapService::UpdateMap operation.
 *
 * It encapsulates information about the type of error, as well as additional details
 * provided by the CURL library and HTTP response.
 */
struct Error
{
    ErrorCode error_code_ = ErrorCode::Success; /**< The general category of error that occurred */
    std::string msg_; /**< A human-readable error message */
    CURLcode curl_code_ = CURLE_OK; /**< CURL error code providing additional information about CURL errors */
    long http_code_ = 200; /**< HTTP response code providing additional information about HTTP errors */
};
} // namespace download

/**
 * @class MapService
 * @brief This class serves as the main interface for accessing and updating onboard map data.
 *
 * It provides functionality to retrieve map data in WSG84 coordinates, which can be requested by various geographical areas:
 * geo rectangle, corridor or by tile IDs. It also offers Over The Air (OTA) update functionality for map data.
 */
class MapService
{
public:
    /**
     * @brief Alias for the map service download error type.
     */
    using Error = dbs_map::download::Error;

    /**
     * @brief Constructs a MapService object with a given configuration.
     *
     * @param config MapServiceConfig object with settings for the MapService.
     */
    MapService( const MapServiceConfig& config );

    /**
     * @brief Destructor for the MapService class.
     */
    ~MapService( );

    /**
     * @brief Returns the current version of the locally stored map data.
     *
     * @return Version object representing the local map version.
     */
    Version GetLocalMapVersion( ) const;

    /**
     * @brief Returns the latest version of the cloud-stored map data.
     *
     * @return Version object representing the cloud map version.
     */
    Version GetCloudMapVersion( ) const;

    /**
     * @brief Fetches consolidated layer data for a specified geographical rectangle.
     *
     * @param rectangle Geographical rectangle for which to return map data.
     * @return A shared pointer to a ConsolidatedLayers object containing the requested map data.
     */
    model::ConsolidatedLayers::Ptr GetLayersForRectangle( const GeoRectangle& rectangle ) const;

    /**
     * @brief Fetches consolidated layer data for a specified list of tile partitions.
     *
     * @param tile_ids A list of PartitionId's for which to return map data.
     * @return A shared pointer to a ConsolidatedLayers object containing the requested map data.
     */
    model::ConsolidatedLayers::Ptr GetLayersForTiles( const std::vector< PartitionId >& tile_ids ) const;

    /**
     * @brief Fetches consolidated layer data for a specified corridor.
     *
     * @param polyline Centerline of the corridor for which to return map data.
     * @param corridor_width_in_meters Width of the corridor in meters.
     * @return A shared pointer to a ConsolidatedLayers object containing the requested map data.
     */
    model::ConsolidatedLayers::Ptr GetLayersForCorridor( const std::vector< GeoCoordinates >& polyline, double corridor_width_in_meters ) const;

    /**
     * @brief Fetches the RCA topology data for a specified list of tile partitions.
     *
     * @param tile_ids A list of PartitionId's for which to return map data.
     * @return A shared pointer to an RcaTopology object containing the requested map data.
     */
    model::RcaTopology::Ptr GetRcaTopology( const std::vector< PartitionId >& tile_ids ) const;

    /**
     * @brief Fetches landmark data for a specified list of tile partitions.
     *
     * @param tile_ids A list of PartitionId's for which to return map data.
     * @return A vector of shared pointers to Landmark objects containing the requested map data.
     */
    std::vector< model::Landmark::Ptr > GetLandmarks( const std::vector< PartitionId >& tile_ids ) const;

    /**
     * @brief Fetches zone data for a specified list of tile partitions.
     *
     * @param tile_ids A list of PartitionId's for which to return map data.
     * @return A vector of shared pointers to Zone objects containing the requested map data.
     */
    std::vector< model::Zone::Ptr > GetZones( const std::vector< PartitionId >& tile_ids ) const;

    /**
     * @brief Deletes all locally stored map data.
     *
     */
    void CleanLocalCache( ) const;

    /**
     * @brief Updates the local map data to a specified version.
     *
     * @param version Version object to which the map data will be updated.
     */
    void UpdateLocalMap( const Version& version ) const;

    /**
     * @brief Attempts to update the local map data to a specified version, providing error information.
     *
     * @param version Version object to which the map data will be updated.
     * @return std::vector< Error > List of errors occurred during the update process.
     */
    std::vector< Error > UpdateMap( const Version& version ) const noexcept;

private:
    /**
     * @brief Pointer to the private implementation of the MapService class.
     */
    std::shared_ptr< MapServiceImpl > impl_;
};
} // namespace dbs_map
