/*
 * SPDX-FileCopyrightText: Copyright DB Netz AG
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#include <vector>
#include <string>

namespace map_service
{
namespace download
{
namespace model
{
struct Layer
{
    enum PartitioningType
    {
        Unknown = 0,
        HereTiling,
    };
    std::string catalogId_;
    std::string layerId_;
    std::vector< std::int32_t > zoom_levels_;
    std::string schema_name_;
    PartitioningType partitioning_;
    std::string content_type_;
    std::string content_encoding_;
    // modificationDate
    std::string description_;
};

} // namespace model
} // namespace download
} // namespace map_service
