/*
 * SPDX-FileCopyrightText: Copyright DB Netz AG
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#include <map-service/CommonTypes.h>

namespace map_service
{
struct MapServiceConfig;

namespace utils
{
class MapFileSystem;
} // namespace utils

namespace download
{
class MapUpdater
{
public:
    MapUpdater(
        const std::shared_ptr< MapServiceConfig >& config,
        const std::shared_ptr< utils::MapFileSystem >& map_fs );
    ~MapUpdater( );

    Version GetLocalMapVersion( ) const;
    void CleanLocalCache( ) const;
    void UpdateLocalMap( const Version& to_version ) const;

private:
    void UpdateLayer( const std::string& layer_name, const Version& to_version ) const;

    std::shared_ptr< MapServiceConfig > config_;
    std::shared_ptr< utils::MapFileSystem > map_fs_;
};
} // namespace download
} // namespace map_service
