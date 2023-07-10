/*
 * SPDX-FileCopyrightText: Copyright DB Netz AG
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#include <dbs-map-api/CommonTypes.h>

namespace dbs_map
{
namespace download
{
namespace model
{
/**
 * @struct Partition
 * @brief Data Transfer Object (DTO) representing metadata for a partition in a map layer.
 *
 * This structure is used to transfer metadata about a specific partition within a map layer across
 * different parts of the system or network. The metadata includes the catalog and partition IDs,
 * the catalog version, the data size, checksum, modification date, and data handle of the partition.
 */
struct Partition
{
    /**
     * @brief The ID of the catalog containing the partition.
     *
     * This string represents the unique identifier for the catalog that contains the partition.
     */
    std::string catalog_id_;

    /**
     * @brief The ID of the partition.
     *
     * This represents the unique identifier for the partition within the catalog.
     */
    PartitionId id_;

    /**
     * @brief The version of the catalog containing the partition.
     *
     * This represents the version of the catalog that contains the partition.
     */
    Version catalog_version_;

    /**
     * @brief The data size of the partition.
     *
     * This represents the size (in bytes) of the data in the partition.
     */
    std::uint64_t data_size_;

    /**
     * @brief The checksum of the partition.
     *
     * This is an integer representing the checksum of the partition data. The checksum is
     * a value that is used to check for errors in the data.
     */
    std::int32_t check_sum_;

    /**
     * @brief The modification date of the partition.
     *
     * This is a timestamp (in milliseconds since Unix epoch) representing when the partition was last modified.
     */
    std::uint64_t modification_date_;

    /**
     * @brief The data handle of the partition.
     *
     * This is a string representing the handle of the data in the partition. The data handle is a
     * unique identifier that is used to retrieve the actual data of the partition.
     */
    DataHandle data_handle_;
};

} // namespace model
} // namespace download
} // namespace dbs_map
