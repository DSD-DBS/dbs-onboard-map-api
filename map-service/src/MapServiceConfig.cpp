/*
 * SPDX-FileCopyrightText: Copyright DB Netz AG
 * SPDX-License-Identifier: Apache-2.0
 */

#include <map-service/MapServiceConfig.h>

namespace map_service
{

MapServiceConfig GetDefaultConfig( )
{
    MapServiceConfig result;
    result.zoom_level_ = 12;
    result.http_client_settings_ = download::GetDBClientSettings( );

    return result;
}

} // namespace map_service
