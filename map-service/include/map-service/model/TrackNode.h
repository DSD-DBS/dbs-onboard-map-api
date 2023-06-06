#pragma once
#include <map-service/CommonTypes.h>

namespace map_service
{
namespace model
{
struct TrackNode
{
    using Ptr = std::shared_ptr< const TrackNode >;
    using MutablePtr = std::shared_ptr< TrackNode >;


    enum Type
    {
        Unknown = 0,
        Point,
        SystemBorder,
        BufferStop,
    };

    Id id_;
    Type type_;
};
} // namespace model
} // namespace map_service