/*
 * SPDX-FileCopyrightText: Copyright DB Netz AG
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#include <map-service/model/TrackNode.h>
#include <map-service/model/TrackEdge.h>

namespace map_service
{
namespace model
{
struct RcaTopology
{
    using Ptr = std::shared_ptr< const RcaTopology >;
    using MutablePtr = std::shared_ptr< RcaTopology >;

    std::vector< TrackNode::Ptr > nodes_;
    std::vector< TrackEdge::Ptr > edges_;
};
} // namespace model
} // namespace map_service
