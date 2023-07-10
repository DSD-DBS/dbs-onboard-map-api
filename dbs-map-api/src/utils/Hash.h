/*
 * SPDX-FileCopyrightText: Copyright DB Netz AG
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <cstdint>
#include <filesystem>

namespace dbs_map
{
namespace utils
{
namespace hash
{

std::int32_t FileHash_Murmur3_x86_32( const std::filesystem::path& path );

} // namespace hash
} // namespace utils
} // namespace dbs_map
