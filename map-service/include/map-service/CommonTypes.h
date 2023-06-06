#pragma once

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point.hpp>
#include <boost/geometry/geometries/box.hpp>
#include <boost/geometry/geometries/linestring.hpp>
#include <boost/geometry/geometries/polygon.hpp>

#include <string>
#include <memory>
#include <vector>

namespace map_service
{

using PartitionId = std::string; // // HERE Tile ID
using Version = std::uint64_t;
using DataHandle = std::string;
using Id = std::string;
using JourneyProfile = std::vector< Id >;

using GeoCoordinates = boost::geometry::model::point< double, 2, boost::geometry::cs::geographic< boost::geometry::degree > >;
using GeoCoordinates3d = boost::geometry::model::point< double, 3, boost::geometry::cs::geographic< boost::geometry::degree > >;

using Polyline = boost::geometry::model::linestring< GeoCoordinates >;
using Polyline3d = boost::geometry::model::linestring< GeoCoordinates3d >;

using Polygon = boost::geometry::model::polygon< GeoCoordinates >;
using Polygon3d = boost::geometry::model::polygon< GeoCoordinates3d >;

using GeoRectangle = boost::geometry::model::box< GeoCoordinates >;
using GeoRectangle3d = boost::geometry::model::box< GeoCoordinates3d >;

} // namespace map_service
