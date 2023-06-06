#include "RcaTopologyDecoder.h"

#include <decoder/LayerDecoder.h>
#include <map-service/MapServiceConfig.h>
#include <utils/Geo.h>

#include "org/db/schema/rca/topology/v2/base_layer.pb.h"
#include "org/db/schema/rca/centerline/v2/centerline_layer.pb.h"

namespace map_service
{
namespace decoder
{
using namespace model;
using namespace utils::geo;

namespace proto = org::db::schema::rca::topology::v2;
namespace proto_geometry = org::db::schema::rca::centerline::v2;

namespace
{
auto node_from_id = []( const auto& id )
{
    return std::make_shared< TrackNode >( TrackNode{ id, TrackNode::Unknown } );
};

const auto get_node_type = []( const auto& src )
{
    switch ( src )
    {
        case proto::TRACK_NODE_TYPE_POINT:
            return TrackNode::Point;

        case proto::TRACK_NODE_TYPE_SYSTEM_BORDER:
            return TrackNode::SystemBorder;

        case proto::TRACK_NODE_TYPE_BUFFER_STOP:
            return TrackNode::BufferStop;

        case proto::TRACK_NODE_TYPE_UNKNOWN:
        case proto::TrackNodeType_INT_MIN_SENTINEL_DO_NOT_USE_:
        case proto::TrackNodeType_INT_MAX_SENTINEL_DO_NOT_USE_:
            return TrackNode::Unknown;
    };
    return TrackNode::Unknown;
};

auto to_node = []( const auto& src )
{
    return std::make_shared< TrackNode >( TrackNode{
        src.track_node_id( ),
        get_node_type( src.track_node_type( ) ) } );
};

const auto to_track = []( const auto& src )
{
    return std::make_shared< TrackEdge >( TrackEdge{
        src.track_edge_id( ),
        node_from_id( src.start_node_ref( ).id( ) ),
        node_from_id( src.end_node_ref( ).id( ) ),
    } );
};

const auto get_track_id = []( const auto& item )
{
    return item.track_edge_id( );
};

const auto get_node_id = []( const auto& item )
{
    return item.track_node_id( );
};

const auto resolve_node = []( const auto& node_map, auto& node_to_resolve_ptr )
{
    const auto found_it = node_map.find( node_to_resolve_ptr->id_ );
    if ( found_it == node_map.cend( ) )
    {
        std::cerr << "Error: node with id: " << node_to_resolve_ptr->id_ << " is not found." << std::endl;
    }
    node_to_resolve_ptr = found_it->second;
};

const auto to_geo_coordinate_3d = []( const auto& center_point )
{
    return GeoCoordinates3d( center_point.longitude( ), center_point.latitude( ), center_point.altitude( ) );
};

const auto get_polyline_3d = []( const auto& src )
{
    const auto& geometry = src;

    Polyline3d result;
    result.reserve( geometry.size( ) );
    std::transform( geometry.begin( ), geometry.end( ), std::back_inserter( result ), to_geo_coordinate_3d );

    return result;
};

const auto add_track_geometry = []( auto& dst_map, const auto& edge )
{
    if ( !dst_map.emplace( edge.track_edge_id( ), get_polyline_3d( edge.propertypoints( ) ) ).second )
    {
        std::cerr << "Error: Edge with id: " << edge.track_edge_id( ) << " occured more then once in geometry collections. " << std::endl;
    }
};

} // namespace

RcaTopologyDecoder::RcaTopologyDecoder( const MapServiceConfig& config )
: decoder_( std::make_unique< LayerDecoder >( config, config.catalog_, config.layer_rca_topoloy_ ) ), centerline_decoder_( std::make_unique< LayerDecoder >( config, config.catalog_, config.layer_centerline_ ) )
{
}

RcaTopologyDecoder::~RcaTopologyDecoder( ) = default;

model::RcaTopology::Ptr
RcaTopologyDecoder::ConstructLayer( const std::vector< PartitionId >& tile_ids ) const
{
    std::vector< TrackEdge::MutablePtr > edges;
    std::vector< TrackNode::MutablePtr > nodes;

    Intersections intersections, node_refs;
    const auto fn = [ &node_refs ]( const auto& src )
    {
        // TODO: start node should be always in tile. no need to resolve ref
        // somehow, some start nodes belong to another tile.
        AddRef( src.start_node_ref( ), node_refs );
        AddRef( src.end_node_ref( ), node_refs );

        return to_track( src );
    };

    for ( const auto& decoded : decoder_->DecodeTiles< proto::BaseLayerTile >( tile_ids ) )
    {
        const auto& tile = *decoded.second;
        TransformCollection( tile.track_edges( ), edges, fn );
        TransformCollection( tile.track_nodes( ), nodes, to_node );
        CollectRefs( tile.track_edge_refs( ), intersections );
    }
    for ( const auto& id : tile_ids )
    {
        intersections.erase( id );
    }

    // Resolve TrackEdge references
    for ( const auto& decoded : decoder_->DecodeIntersections< proto::BaseLayerTile >( intersections ) )
    {
        ResolveRefs(
            decoded.second->track_edges( ), edges,
            intersections[ decoded.first ], get_track_id, fn );
    }
    for ( const auto& id : tile_ids )
    {
        node_refs.erase( id );
    }

    // Resolve TrakcNode references
    for ( const auto& decoded : decoder_->DecodeIntersections< proto::BaseLayerTile >( node_refs ) )
    {
        ResolveRefs(
            decoded.second->track_nodes( ), nodes,
            node_refs[ decoded.first ], get_node_id, to_node );
    }
    // Populating side_a and side_b by pointers from RcaTopology::nodes_ collections.
    // Each node has one smart_ptr instance in this case.
    std::unordered_map< Id, TrackNode::Ptr > node_map;
    for ( const auto& node_ptr : nodes )
    {
        if ( !node_map.emplace( node_ptr->id_, node_ptr ).second )
        {
            std::cerr << "Error: node with id: " << node_ptr->id_ << " occured more then once in collection. " << std::endl;
        }
    }

    for ( const auto& edge_ptr : edges )
    {
        resolve_node( node_map, edge_ptr->side_a_ );
        resolve_node( node_map, edge_ptr->side_b_ );
    }

    // Get geometry
    std::unordered_map< Id, Polyline3d > track_geometries;
    for ( const auto& decoded : centerline_decoder_->DecodeTiles< proto_geometry::CenterlineLayerTile >( tile_ids ) )
    {
        const auto& tile = *decoded.second;
        for ( const auto& edge : tile.trackedges( ) )
        {
            add_track_geometry( track_geometries, edge );
        }
    }

    for ( const auto& decoded : centerline_decoder_->DecodeIntersections< proto_geometry::CenterlineLayerTile >( intersections ) )
    {
        const auto& tile = *decoded.second;
        const auto& edge_ids = intersections[ decoded.first ];
        for ( const auto& edge : tile.trackedges( ) )
        {
            if ( edge_ids.find( edge.track_edge_id( ) ) != edge_ids.cend( ) )
            {
                add_track_geometry( track_geometries, edge );
            }
        }
    }
    for ( const auto& edge_ptr : edges )
    {
        const auto found_it = track_geometries.find( edge_ptr->id_ );
        if ( found_it == track_geometries.cend( ) )
        {
            std::cerr << "Error: No geometry for track with id: " << edge_ptr->id_ << std::endl;
            continue;
        }
        edge_ptr->geometry_ = std::move( found_it->second );
    }

    // TODO: extra copy of vector. src vector has mutable items. dst vector is const items.
    // There is no conversoin from vector with mutable items to vector with const items.
    return std::make_shared< RcaTopology >( RcaTopology{
        std::vector< TrackNode::Ptr >( nodes.cbegin( ), nodes.cend( ) ),
        std::vector< TrackEdge::Ptr >( edges.cbegin( ), edges.cend( ) ),
    } );
}
} // namespace decoder
} // namespace map_service