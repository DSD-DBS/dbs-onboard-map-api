/*
 * SPDX-FileCopyrightText: Copyright DB Netz AG
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#include <map-service/CommonTypes.h>
#include <map-service/download/Response.h>

namespace map_service
{
struct MapServiceConfig;

namespace utils
{
class MapFileSystem;
} // namespace utils

class MapUpdater
{
public:
    using Error = map_service::download::Error;

    MapUpdater(
        const std::shared_ptr< MapServiceConfig >& config,
        const std::shared_ptr< utils::MapFileSystem >& map_fs );
    ~MapUpdater( );

    Version GetLocalMapVersion( ) const;
    void CleanLocalCache( ) const;
    std::vector< Error > UpdateMap( const Version& to_version ) const;

private:
    std::vector< Error > UpdateLayer( const std::string& layer_name, const Version& to_version ) const;

    std::shared_ptr< MapServiceConfig > config_;
    std::shared_ptr< utils::MapFileSystem > map_fs_;
};
} // namespace map_service
