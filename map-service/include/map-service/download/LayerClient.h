/*
 * SPDX-FileCopyrightText: Copyright DB Netz AG
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#include <map-service/download/model/Layer.h>
#include <map-service/download/model/Partition.h>

#include <memory>

namespace map_service
{
namespace download
{

class LayerClientImpl;
struct ClientSettings;

/**
 * @class LayerClient
 * @brief Provides network access to a specific map layer in a DB map catalog.
 *
 * This class is used to interact with a specific layer of a DB map catalog over the network. It allows
 * retrieving metadata for the layer, the partition metadata for a specific partition ID, retrieving partition content via data handle
 * , getting the differences between two versions of the layer and retrieving all partitions metadata.
 */
class LayerClient
{
public:
    /**
     * @brief Constructs a LayerClient object.
     *
     * @param[in] catalog_name The name of the catalog.
     * @param[in] layer_name The name of the layer.
     * @param[in] catalog_version The version of the catalog.
     * @param[in] settings Client settings for network access configuration.
     */
    LayerClient( const std::string& catalog_name, const std::string& layer_name,
        Version catalog_version, const ClientSettings& settings );

    /**
     * @brief Retrieves the metadata for the layer.
     *
     * This method returns a Layer object that contains the metadata of the layer.
     *
     * @return The Layer object that contains the requested metadata.
     */
    model::Layer GetMetadata( ) const;

    /**
     * @brief Retrieves the partition metadata for a specified ID.
     *
     * @param[in] id The ID of the partition for which metadata is needed.
     * @return The Partition object that contains the requested metadata.
     */
    model::Partition GetPartitionMetadata( const PartitionId& id ) const;

    /**
     * @brief Retrieves content of the partition via data handle.
     *
     * @param[in] data_handle The handle for the data to be retrieved.
     * @param[out] out The output stream where the data will be written.
     */
    void GetDataHandle( const DataHandle& data_handle, std::ostream& out ) const;

    /**
     * @brief Retrieves the differences between two versions of the layer.
     *
     * This method returns a vector of Partition objects, each representing a partition that has changed
     * from the provided version.
     *
     * @param[in] from_version The version of the layer to compare from.
     * @return A vector of Partition objects, each representing a changed partition.
     */
    std::vector< model::Partition > GetDifference( const Version& from_version ) const;

    /**
     * @brief Retrieves metadata for all partitions of the layer.
     *
     * This method returns a vector of Partition objects, each representing the metadata for a partition of the layer.
     *
     * @return A vector of Partition objects, each representing a partition of the layer.
     */
    std::vector< model::Partition > GetAllPartitionsMetadata( ) const;

private:
    /**
     * @brief Pointer to the implementation of the LayerClient class.
     *
     * This class uses the Pimpl idiom (pointer to implementation) to hide the details of its implementation
     * and to provide a clean interface.
     */
    std::shared_ptr< LayerClientImpl > impl_;
};

} // namespace download
} // namespace map_service
