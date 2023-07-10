/*
 * SPDX-FileCopyrightText: Copyright DB Netz AG
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point.hpp>
#include <boost/geometry/geometries/box.hpp>
#include <boost/geometry/geometries/linestring.hpp>
#include <boost/geometry/geometries/polygon.hpp>

#include <string>
#include <memory>
#include <vector>

namespace dbs_map
{

/**
 * @typedef PartitionId
 * @brief Represents Partition (tile) ID in the map service.
 */
using PartitionId = std::string;

/**
 * @typedef Version
 * @brief Represents a map version as a 64-bit unsigned integer.
 */
using Version = std::uint64_t;

/**
 * @typedef DataHandle
 * @brief Represents a unique identifier for data in the map service.
 */
using DataHandle = std::string;

/**
 * @typedef Id
 * @brief Represents a generic unique identifier.
 */
using Id = std::string;

/**
 * @typedef GeoCoordinates
 * @brief Represents a geographic point on the Earth surface, expressed in geographic coordinate system
 * with latitude and longitude values in degrees. Implemented using Boost.Geometry library.
 */
using GeoCoordinates = boost::geometry::model::point< double, 2, boost::geometry::cs::geographic< boost::geometry::degree > >;

/**
 * @typedef GeoCoordinates3d
 * @brief Represents a geographic point in 3D space, expressed in geographic coordinate system
 * with latitude, longitude, and altitude values in degrees. Implemented using Boost.Geometry library.
 */
using GeoCoordinates3d = boost::geometry::model::point< double, 3, boost::geometry::cs::geographic< boost::geometry::degree > >;

/**
 * @typedef Polyline
 * @brief Represents a sequence of geographic points on the Earth surface, forming a polyline.
 * Implemented using Boost.Geometry library.
 */
using Polyline = boost::geometry::model::linestring< GeoCoordinates >;

/**
 * @typedef Polyline3d
 * @brief Represents a sequence of geographic points in 3D space, forming a polyline.
 * Implemented using Boost.Geometry library.
 */
using Polyline3d = boost::geometry::model::linestring< GeoCoordinates3d >;

/**
 * @typedef Polygon
 * @brief Represents a polygon on the Earth surface, defined by a sequence of geographic points.
 * Implemented using Boost.Geometry library.
 */
using Polygon = boost::geometry::model::polygon< GeoCoordinates >;

/**
 * @typedef Polygon3d
 * @brief Represents a polygon in 3D space, defined by a sequence of geographic points.
 * Implemented using Boost.Geometry library.
 */
using Polygon3d = boost::geometry::model::polygon< GeoCoordinates3d >;

/**
 * @typedef GeoRectangle
 * @brief Represents a rectangle on the Earth surface, defined by its minimum and maximum geographic points.
 * Implemented using Boost.Geometry library.
 */
using GeoRectangle = boost::geometry::model::box< GeoCoordinates >;

/**
 * @typedef GeoRectangle3d
 * @brief Represents a rectangular box in 3D space, defined by its minimum and maximum geographic points.
 * Implemented using Boost.Geometry library.
 */
using GeoRectangle3d = boost::geometry::model::box< GeoCoordinates3d >;

} // namespace dbs_map
