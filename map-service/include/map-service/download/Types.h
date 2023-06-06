#pragma once
#include <map-service/download/Response.h>
#include <map-service/download/model/Partition.h>
#include <map-service/download/model/Catalog.h>
#include <map-service/download/model/Layer.h>

#include <map-service/CommonTypes.h>

namespace map_service
{
namespace download
{

using PartitionResponse = Response< model::Partition >;
using CatalogResponse = Response< model::Catalog >;
using LayerResponse = Response< model::Layer >;
using PartitionListResponse = Response< std::vector< model::Partition > >;

using Version = map_service::Version;
using PartitionId = map_service::PartitionId;
using DataHandle = map_service::DataHandle;

} // namespace download
} // namespace map_service
