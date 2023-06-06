#pragma once
#include <map-service/CommonTypes.h>
#include <variant>
#include "SimplifiedLandmarks.h"

namespace map_service
{
namespace model
{
struct SurfaceWithHeight
{
    Polygon3d surface_;
    std::uint64_t height_cm;
};
struct Landmark
{
    using Ptr = std::shared_ptr< const Landmark >;
    using MutablePtr = std::shared_ptr< Landmark >;
    using Geometry = std::variant< Polyline3d, Polygon3d, SurfaceWithHeight >;

    enum Type
    {
        Unknown = 0,
        CatenaryPole,
        Platform,
        Wall,
        SignPole,
        SignalPole,
        BufferStop,
        Billboard,
        BillboardPole,
        BridgePillar,
        Building,
        CameraPole,
        Container,
        CoveredPlatform,
        FuseBox,
        LightPole,
        OtherPole,
        Shelter,
        TrashCan,
        TunnelWall,
        BarrierBar,
        BarrierBody,
    };

    Type type_;
    Id id_;
    Geometry geometry_;
};

} // namespace model
} // namespace map_service