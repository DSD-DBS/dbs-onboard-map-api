#pragma once
#include <map-service/download/Response.h>
#include <rapidjson/document.h>

#include <memory>

namespace map_service
{
namespace download
{
class ClientSettings;
} // namespace download

namespace utils
{
class HttpClient
{
public:
    using Error = download::Error;
    using ErrorCode = download::ErrorCode;
    using ClientSettings = download::ClientSettings;

    HttpClient( const ClientSettings& settings );
    ~HttpClient( );

    Error GetUri( const std::string& url, std::ostream& out ) const;
    Error GetUri( const std::string& url, rapidjson::Document& out ) const;

private:
    std::shared_ptr< ClientSettings > client_settings_;
};
} // namespace utils
} // namespace map_service
