#pragma once
#include <map-service/model/RcaTopology.h>
#include <map-service/model/Landmarks.h>
#include <map-service/model/Zone.h>

namespace map_service
{
namespace model
{
struct ConsolidatedLayers
{
public:
    using Ptr = std::shared_ptr< const ConsolidatedLayers >;
    using MutablePtr = std::shared_ptr< ConsolidatedLayers >;

    RcaTopology::Ptr topology_;
    std::vector< Landmark::Ptr > landmarks_;
    std::vector< Zone::Ptr > zones_;

    std::vector< PartitionId > partition_ids_;
};
} // namespace model
} // namespace map_service