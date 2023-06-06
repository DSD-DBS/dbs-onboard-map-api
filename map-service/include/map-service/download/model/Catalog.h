#pragma once
#include <map-service/CommonTypes.h>
#include <vector>

namespace map_service
{
namespace download
{
namespace model
{
struct Catalog
{
    std::string id_;
    map_service::Version version_;
    map_service::Version latest_version_;
    std::uint64_t modification_date_;
    std::string description_;
    std::vector< std::string > layers_;
};

} // namespace model
} // namespace download
} // namespace map_service