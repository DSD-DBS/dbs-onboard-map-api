/*
 * SPDX-FileCopyrightText: Copyright DB Netz AG
 * SPDX-License-Identifier: Apache-2.0
 */

#include <map-service/MapService.h>
#include <map-service/MapServiceConfig.h>

#include <map-service/download/LayerClient.h>

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <vector>
#include <algorithm>
#include <unistd.h>
#include <filesystem>

namespace
{
using namespace map_service;
using namespace map_service::model;

MapServiceConfig TestConfig( )
{
    MapServiceConfig result = GetDefaultConfig( );
    result.map_local_path_ = std::filesystem::current_path( ) / "build" / "hdmap";
    return result;
}

const std::vector< std::string > all_zone_partitions =
    {
        "23608580",
        "23608581",
        "23608582",
        "23608592",
};


const std::vector< std::string > all_landmark_partitions = {
    "23608580",
    "23608581",
    "23608582",
    "23608592",
};

const auto find_by_type = []( const auto& src, const auto& type )
{
    const auto fn = [ &type ]( const auto& item )
    {
        return item->type_ == type;
    };
    return std::find_if( src.cbegin( ), src.cend( ), fn );
};

const auto find_by_id = []( const auto& src, const std::string& id )
{
    const auto fn = [ &id ]( const auto& item )
    {
        return item->id_ == id;
    };
    return std::find_if( src.cbegin( ), src.cend( ), fn );
};

} // unnamed namespace

TEST( MapService, DISABLED_TestGetCloudMapVersion )
{
    // Arrange
    auto config = TestConfig( );

    // Act / Assert
    ASSERT_GT( MapService( config ).GetCloudMapVersion( ), 0 );
}

TEST( MapService, TestGetLocalMapVersion )
{
    // Arrange
    auto config = TestConfig( );

    // Act / Assert
    ASSERT_EQ( MapService( config ).GetLocalMapVersion( ), 2 );
}


TEST( MapService, GetLayersForRectangle )
{
    // Arrange
    MapService service( TestConfig( ) );

    GeoRectangle box(
        GeoCoordinates( 9.4658, 53.3921 ),
        GeoCoordinates( 10.3233, 53.6582 ) );
    // Act
    const auto result = service.GetLayersForRectangle( box );

    // Asserts
    ASSERT_NE( result, nullptr );
    ASSERT_NE( result->topology_, nullptr );
    ASSERT_NE( result->topology_->edges_.size( ), 0 );
    ASSERT_NE( result->topology_->nodes_.size( ), 0 );


    ASSERT_NE( result->landmarks_.size( ), 0 );
    ASSERT_NE( result->landmarks_.front( ), nullptr );

    ASSERT_NE( result->zones_.size( ), 0 );
    ASSERT_NE( result->zones_.front( ), nullptr );
}

TEST( MapService, GetRcaTopology )
{
    // Arrange
    MapService service( TestConfig( ) );

    // Act
    const auto result = service.GetRcaTopology( {
        // "23608580",
        "23608581",
        "23608582",
        // "23608592",
    } );


    // Asserts
    ASSERT_NE( result, nullptr );
    ASSERT_NE( result->edges_.size( ), 0 );
    ASSERT_NE( result->nodes_.size( ), 0 );

    // Nodes
    const auto& nodes = result->nodes_;

    auto found_it = find_by_type( nodes, TrackNode::Point );
    ASSERT_NE( found_it, nodes.end( ) );

    auto node_ptr = *found_it;
    ASSERT_NE( node_ptr, nullptr );
    ASSERT_NE( node_ptr->id_, "" );
    ASSERT_EQ( node_ptr->type_, TrackNode::Point );

    //  Edges
    const auto& edges = result->edges_;

    auto found = edges.begin( );
    auto edge_ptr = *found;
    ASSERT_NE( edge_ptr, nullptr );

    ASSERT_NE( edge_ptr->side_a_, nullptr );
    ASSERT_NE( edge_ptr->side_a_->id_, "" );
    ASSERT_NE( edge_ptr->side_a_->type_, TrackNode::Unknown );

    ASSERT_NE( edge_ptr->side_b_, nullptr );
    ASSERT_NE( edge_ptr->side_b_->id_, "" );
    ASSERT_NE( edge_ptr->side_b_->type_, TrackNode::Unknown );
    ASSERT_NE( edge_ptr->geometry_.size( ), 0 );

    // Check if node references was resolved.
    auto node_id = "39d988ca-8039-3760-8714-6b19f8db9d63"; // tile_id = 23608592
    auto node_it = find_by_id( nodes, node_id );
    ASSERT_NE( node_it, nodes.end( ) );

    node_ptr = *node_it;
    ASSERT_NE( node_ptr, nullptr );
    ASSERT_EQ( node_ptr->id_, node_id );
    ASSERT_NE( node_ptr->type_, TrackNode::Unknown );

    // But extra nodes are not loaded
    node_id = "f352999a-a926-32bb-a472-3dfc7d08d586"; // tile id = 23608592
    node_it = find_by_id( nodes, node_id );
    ASSERT_EQ( node_it, nodes.end( ) );

    // Check if edge references was resolved.
    auto edge_id = "5d8d9d2d-13fa-3d98-8041-84f9ee272d61"; // tile id = 23608580
    auto edge_it = find_by_id( edges, edge_id );
    ASSERT_NE( edge_it, edges.cend( ) );

    edge_ptr = *edge_it;
    ASSERT_NE( edge_ptr, nullptr );
    ASSERT_EQ( edge_ptr->id_, edge_id );
    ASSERT_NE( edge_ptr->side_a_, nullptr );
    ASSERT_NE( edge_ptr->side_a_->id_, "" );
    ASSERT_NE( edge_ptr->side_a_->type_, TrackNode::Unknown );

    ASSERT_NE( edge_ptr->side_b_, nullptr );
    ASSERT_NE( edge_ptr->side_b_->id_, "" );
    ASSERT_NE( edge_ptr->side_b_->type_, TrackNode::Unknown );
    ASSERT_NE( edge_ptr->geometry_.size( ), 0 );

    // But extra edges are not loaded from the same tile where reference is anchored.
    edge_id = "223f56ef-d379-3b6b-b680-2917abeda43c"; // tile id = 23608580
    edge_it = find_by_id( edges, edge_id );
    ASSERT_EQ( edge_it, edges.end( ) );
}

TEST( MapService, Landmarks )
{
    // Arrange
    std::vector< Landmark::Type > landmark_types =
        {
            Landmark::CatenaryPole,
            Landmark::Platform,
            Landmark::Wall,
            Landmark::SignPole,
            Landmark::SignalPole,
            // Landmark::BufferStop,
            Landmark::Billboard,
            Landmark::BridgePillar,
            // TODO: HDMAP-1015: fix height. Landmark::Building,
            Landmark::CameraPole,
            // TODO: HDMAP-1015: fix height. Landmark::Container,
            Landmark::CoveredPlatform,
            // TODO: HDMAP-1015: fix height. Landmark::FuseBox,
            Landmark::LightPole,
            Landmark::OtherPole,
            Landmark::Shelter,
            // Landmark::TrashCan,
            // Landmark::TunnelWall,
            // Landmark::BarrierBar,
            // Landmark::BarrierBody,
        };

    MapService service( TestConfig( ) );

    // Act
    // const auto partitions = { "23608600", "23608600" };
    const auto result = service.GetLandmarks( all_landmark_partitions );

    // Assert
    ASSERT_GT( result.size( ), 0 );

    for ( const auto& landmark_type : landmark_types )
    {
        const auto found_it = find_by_type( result, landmark_type );
        ASSERT_NE( found_it, result.cend( ) ) << "Landmark type: " << landmark_type << std::endl;
        ASSERT_NE( *found_it, nullptr );

        const auto& landmark_ptr = *found_it;
        ASSERT_NE( landmark_ptr->id_.size( ), 0 );

        switch ( landmark_type )
        {
            case Landmark::Platform:
            case Landmark::Building:
            case Landmark::Container:
            case Landmark::FuseBox:
            case Landmark::TrashCan:
            case Landmark::BarrierBody:
            {
                const auto* geometry = std::get_if< SurfaceWithHeight >( &landmark_ptr->geometry_ );
                ASSERT_NE( geometry, nullptr );
                ASSERT_GT( geometry->height_cm, 0 ) << "incorrect landmark: " << landmark_ptr->id_ << ", height: " << geometry->height_cm;
                ASSERT_TRUE( !boost::geometry::is_empty( geometry->surface_ ) );
            }
            break;

            case Landmark::Wall:
            case Landmark::TunnelWall:
            {
                const auto* geometry_ptr = std::get_if< Polygon3d >( &landmark_ptr->geometry_ );
                ASSERT_NE( geometry_ptr, nullptr );

                const auto& start = geometry_ptr->outer( ).front( );
                const auto& back = geometry_ptr->outer( ).back( );

                // TODO: investigate why aseerts are failing
                // const double eps = 0.00000000000001;
                // ASSERT_NEAR( start.get< 0 >( ), back.get< 0 >( ), eps );
                // ASSERT_NEAR( start.get< 1 >( ), back.get< 1 >( ), eps );
                // ASSERT_NEAR( start.get< 2 >( ), back.get< 2 >( ), eps );

                // ASSERT_GT( geometry_ptr->height_, 0 );
            }
            break;

            case Landmark::Billboard:
            {
                const auto* geometry_ptr = std::get_if< Polygon3d >( &landmark_ptr->geometry_ );
                ASSERT_NE( geometry_ptr, nullptr );
                // TODO: provide geometry checks
            }
            break;


            case Landmark::BillboardPole:
            {
                const auto* geometry_ptr = std::get_if< Polyline3d >( &landmark_ptr->geometry_ );
                ASSERT_NE( geometry_ptr, nullptr );
                // TODO: provide geometry checks
            }
            break;

            case Landmark::CatenaryPole:
            case Landmark::SignPole:
            case Landmark::SignalPole:
            case Landmark::CameraPole:
            case Landmark::LightPole:
            case Landmark::OtherPole:
            case Landmark::BufferStop:
            case Landmark::BridgePillar:
            case Landmark::CoveredPlatform:
            case Landmark::Shelter:
            case Landmark::BarrierBar:
            case Landmark::Unknown:
            {
                ASSERT_NE( std::get_if< Polyline3d >( &landmark_ptr->geometry_ ), nullptr );
            }
            break;
        };
    }
}

TEST( MapService, GetZones )
{
    // Arrange
    using namespace map_service;
    using namespace model;

    MapService service( TestConfig( ) );

    std::vector< std::string > partitions = all_zone_partitions; //{ "23608600", "23608602" };
    // Act
    const auto result = service.GetZones( partitions );

    // Assert
    ASSERT_GT( result.size( ), 0 );

    ASSERT_NE( find_by_type( result, Zone::RiskZone ), result.cend( ) );
    ASSERT_NE( find_by_type( result, Zone::SafeZone ), result.cend( ) );
    // ASSERT_NE( find_by_type( result, Zone::LevelCrossingZone ), result.cend( ) );


    const auto zone_ptr = *result.cbegin( );
    ASSERT_NE( zone_ptr, nullptr );

    ASSERT_NE( zone_ptr->type_, model::Zone::Unknown );

    const auto& begin_it = zone_ptr->geometry_.outer( ).begin( );
    const auto& end_it = zone_ptr->geometry_.outer( ).end( );

    ASSERT_NE( begin_it, end_it );

    const auto& start = *begin_it;
    const auto& back = *std::prev( end_it );

    ASSERT_DOUBLE_EQ( start.get< 0 >( ), back.get< 0 >( ) );
    ASSERT_DOUBLE_EQ( start.get< 1 >( ), back.get< 1 >( ) );
    ASSERT_DOUBLE_EQ( start.get< 2 >( ), back.get< 2 >( ) );

    // ASSERT_GT( start.get< 0 >( ), 10.0 );
    // ASSERT_GT( start.get< 1 >( ), 53.0 );
    //  TODO: check elevation ASSERT_EQ( start.get< 2 >( ), 5130 );
}

// TEST( MapService, PlaneStructureIntersection )
// {
//     // Arrange
//     using namespace map::service;
//     MapService service( TestConfig( ) );

//     // Act
//     const auto result = service.GetPlaneStructures( {"377737486"} );

//     // Assert
//     ASSERT_EQ( result.size( ), 407 + 2 );

//     auto fn = []( const model::PlaneStructure::Ptr& s ) { return s->id_ == 34174; };
//     ASSERT_NE( std::find_if( result.cbegin( ), result.cend( ), fn ), result.cend( ) );
// }
