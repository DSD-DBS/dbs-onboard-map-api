/*
 * SPDX-FileCopyrightText: Copyright DB Netz AG
 * SPDX-License-Identifier: Apache-2.0
 */

#include <dbs-map-api/download/ClientSettings.h>

namespace dbs_map
{
namespace download
{

ClientSettings GetDBClientSettings( )
{
    return {
        "https", // protocol_
        "map-cdb-api-server-dev.digitalregister.az4db-iat.comp.db.de", // host_
    };
}

ClientSettings GetDBClientForCISettings( )
{
    return {
        // "map-cdb-api-server-dev.drase-dev.appserviceenvironment.net", // host_
        "http",
        "localhost:8080"
    };
}

} // namespace download
} // namespace dbs_map
