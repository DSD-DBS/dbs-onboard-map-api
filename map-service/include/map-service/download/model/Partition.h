/*
 * SPDX-FileCopyrightText: Copyright DB Netz AG
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#include <map-service/CommonTypes.h>

namespace map_service
{
namespace download
{
namespace model
{
struct Partition
{
    std::string catalog_id_;
    map_service::PartitionId id_; // blobKey
    map_service::Version catalog_version_;

    std::uint64_t data_size_;
    std::int32_t check_sum_;
    std::uint64_t modification_date_;
    map_service::DataHandle data_handle_;
};

} // namespace model
} // namespace download
} // namespace map_service
