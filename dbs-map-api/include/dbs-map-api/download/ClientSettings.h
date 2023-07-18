
/*
 * SPDX-FileCopyrightText: Copyright DB Netz AG
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#include <string>
#include <list>

namespace dbs_map
{
namespace download
{

/**
 * @struct ClientSettings
 * @brief Configuration settings for the HTTP client.
 *
 * This structure contains the settings that are used to configure the HTTP client for
 * network access. These settings include the host, HTTP headers, and verbosity of logs.
 */
struct ClientSettings
{
    /**
     * @brief The protocol for the HTTP client.
     *
     * This string specifies the protocol which the HTTP client will use to connect.
     */
    std::string protocol_ = "https";

    /**
     * @brief The host for the HTTP client.
     *
     * This string specifies the host to which the HTTP client will connect.
     */
    std::string host_;

    /**
     * @brief The HTTP headers for the client.
     *
     * This list contains the HTTP headers that will be included in the HTTP requests
     * made by the client.
     */
    std::list< std::string > http_headers_;

    /**
     * @brief Indicates whether verbose logging is enabled.
     *
     * If this boolean is set to true, then the client will log detailed information
     * about its operations.
     */
    bool verbose_ = false;
};

/**
 * @brief Constructs and returns a ClientSettings object for the DB client.
 *
 * This function is used to construct a ClientSettings object with the appropriate settings
 * for the DB client. The constructed object is then returned.
 *
 * @return A ClientSettings object with the settings for the DB client.
 */
ClientSettings GetDBClientSettings( );

/**
 * @brief Constructs and returns a ClientSettings object for the DB client for CI.
 *
 * This function is used to construct a ClientSettings object with the appropriate settings
 * for the DB client for Continuous Integration (CI). The constructed object is then returned.
 *
 * @return A ClientSettings object with the settings for the DB client for CI.
 */
ClientSettings GetDBClientForCISettings( );


} // namespace download
} // namespace dbs_map
