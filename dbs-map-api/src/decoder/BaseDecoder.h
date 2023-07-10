/*
 * SPDX-FileCopyrightText: Copyright DB Netz AG
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#include <dbs-map-api/CommonTypes.h>

namespace dbs_map
{
namespace decoder
{
template < class TResult >
class BaseDecoder
{
public:
    virtual ~BaseDecoder( ) = default;

    virtual TResult ConstructLayer( const std::vector< PartitionId >& tiles ) const = 0;
};

} // namespace decoder
} // namespace dbs_map
