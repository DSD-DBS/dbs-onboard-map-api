/*
 * SPDX-FileCopyrightText: Copyright DB Netz AG
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#include <map-service/CommonTypes.h>
#include <map-service/model/TrackNode.h>

namespace map_service
{
namespace model
{
struct TrackEdge
{
    using Ptr = std::shared_ptr< const TrackEdge >;
    using MutablePtr = std::shared_ptr< TrackEdge >;

    Id id_;
    TrackNode::Ptr side_a_;
    TrackNode::Ptr side_b_;
    Polyline3d geometry_;
};
} // namespace model
} // namespace map_service
