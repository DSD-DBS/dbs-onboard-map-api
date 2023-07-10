/*
 * SPDX-FileCopyrightText: Copyright DB Netz AG
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#include <dbs-map-api/download/model/Catalog.h>
#include <memory>

namespace map_service
{
namespace download
{
class CatalogClientImpl;
struct ClientSettings;

/**
 * @class CatalogClient
 * @brief Provides network access to a Deutsche Bahn (DB) map catalog.
 *
 * This class is used to interact with a DB map catalog over the network. It allows to retrieve the
 * metadata for a specified version of the catalog and also to get the latest version of the catalog.
 */
class CatalogClient
{
public:
    /**
     * @brief Constructs a CatalogClient object.
     *
     * @param[in] catalog_name The name of the catalog.
     * @param[in] settings Client settings for network access configuration.
     */
    CatalogClient( const std::string& catalog_name, const ClientSettings& settings );

    /**
     * @brief Retrieves the metadata for a specified version of the catalog.
     *
     * This method returns a Catalog object that contains the metadata of the catalog for the provided version.
     *
     * @param[in] version The version of the catalog for which metadata is needed.
     * @return The Catalog object that contains the requested metadata.
     */
    model::Catalog GetMetadata( map_service::Version version ) const;

    /**
     * @brief Retrieves the latest version of the catalog.
     *
     * This method returns the most recent version available for the catalog.
     *
     * @return The most recent version of the catalog.
     */
    map_service::Version GetLatestVersion( ) const;

private:
    /**
     * @brief Pointer to the implementation of the CatalogClient class.
     *
     * This class uses the Pimpl idiom (pointer to implementation) to hide the details of its implementation
     * and to provide a clean interface.
     */
    std::shared_ptr< CatalogClientImpl > impl_;
};

} // namespace download
} // namespace map_service
