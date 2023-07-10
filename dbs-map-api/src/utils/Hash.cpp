/*
 * SPDX-FileCopyrightText: Copyright DB Netz AG
 * SPDX-License-Identifier: Apache-2.0
 */

#include "Hash.h"
#include <smasher/src/Hashes.h>

#include <fstream>

namespace dbs_map
{
namespace utils
{
namespace hash
{

std::int32_t FileHash_Murmur3_x86_32( const std::filesystem::path& path )
{
    const std::uint32_t seed = 104729;
    std::int32_t result = 0;

    std::ifstream file( path, std::ios::binary | std::ios::ate );
    const auto size = file.tellg( );

    file.seekg( 0, std::ios::beg );

    std::vector< char > content( size );
    if ( file.read( content.data( ), size ) )
    {
        MurmurHash3_x86_32( content.data( ), content.size( ), seed, &result );
    }

    return result;
}

} // namespace hash
} // namespace utils
} // namespace dbs_map
