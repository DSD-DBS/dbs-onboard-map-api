/*
 * SPDX-FileCopyrightText: Copyright DB Netz AG
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#include <dbs-map-api/CommonTypes.h>
#include <dbs-map-api/utils/MapFileSystem.h>

#include <utils/Geo.h>

#include <unordered_map>
#include <fstream>
#include <iostream>

namespace map_service
{
struct MapServiceConfig;

namespace decoder
{
/**
 * @brief Decoding several tiles from any protobuf layer.
 */
class LayerDecoder
{
public:
    LayerDecoder( const MapServiceConfig& config,
        const std::string& catalog,
        const std::string& layer );

    ~LayerDecoder( ) = default;

    template < class T >
    std::unordered_map< PartitionId, std::shared_ptr< T > >
    DecodeTiles( const std::vector< PartitionId >& tiles ) const
    {
        std::unordered_map< PartitionId, std::shared_ptr< T > > result;
        for ( const auto& tile_id : tiles )
        {
            const auto tile_path = map_fs.GetCurrentMapPartitionPath( layer_, tile_id );
            std::ifstream protobuf_data_stream( tile_path );
            if ( !protobuf_data_stream )
            {
                // std::cerr << "There is no tile: " << tile_path << std::endl;
                continue;
            }

            auto tile = std::make_shared< T >( );
            if ( !tile->ParseFromIstream( &protobuf_data_stream ) )
            {
                std::cerr << "Cannot parse tile " << tile_path << std::endl;
                continue;
            }
            result.emplace( tile_id, tile );
        }
        return result;
    }

    template < class T >
    std::unordered_map< PartitionId, std::shared_ptr< T > >
    DecodeIntersections( const utils::geo::Intersections& inersections ) const
    {
        const auto tile_ids = utils::geo::GetKeys( inersections );
        const auto decoded = DecodeTiles< T >( tile_ids );

        if ( tile_ids.size( ) != decoded.size( ) )
        {
            for ( const auto& id : tile_ids )
            {
                if ( decoded.find( id ) == decoded.end( ) )
                {
                    std::cerr << "Cannot resolve tile: " << map_fs.GetCurrentMapPartitionPath( layer_, id ) << ". Lead to broken references." << std::endl;
                }
            }
        }
        return decoded;
    }

private:
    const MapServiceConfig& config_;
    const std::string catalog_;
    const std::string layer_;
    const utils::MapFileSystem map_fs;
};
} // namespace decoder
} // namespace map_service
