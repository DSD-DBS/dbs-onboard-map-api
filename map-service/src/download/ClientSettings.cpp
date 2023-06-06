#include <map-service/download/ClientSettings.h>

namespace map_service
{
namespace download
{

ClientSettings GetDBClientSettings( )
{
    return {
        "map-cdb-api-server-dev.digitalregister.az4db-iat.comp.db.de", // host_
    };
}

ClientSettings GetDBClientForCISettings( )
{
    return {
        "map-cdb-api-server-dev.drase-dev.appserviceenvironment.net", // host_
    };
}

} // namespace download
} // namespace map_service