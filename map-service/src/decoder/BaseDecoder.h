/*
 * SPDX-FileCopyrightText: Copyright DB Netz AG
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#include <map-service/CommonTypes.h>

namespace map_service
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
} // namespace map_service
