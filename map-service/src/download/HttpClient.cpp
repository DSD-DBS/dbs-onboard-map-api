/*
 * SPDX-FileCopyrightText: Copyright DB Netz AG
 * SPDX-License-Identifier: Apache-2.0
 */

#include <download/HttpClient.h>
#include <map-service/download/ClientSettings.h>
#include <map-service/download/HttpRuntimeError.h>

#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/Infos.hpp>

#include <sstream>

namespace map_service
{
namespace download
{

using namespace curlpp;
using namespace curlpp::options;

namespace
{

size_t
WriteToStream( std::ostream& out, char* in, size_t char_size, size_t length )
{
    const auto buffer_size = char_size * length;
    out.write( in, buffer_size );

    return buffer_size;
}

} // unnamed namespace

HttpClient::HttpClient( const ClientSettings& settings )
: client_settings_( std::make_shared< ClientSettings >( settings ) )
{
}

HttpClient::~HttpClient( )
{
}

void HttpClient::GetUri( const std::string& url, std::ostream& out ) const
{
    // if ( client_settings_->verbose_ )
    {
        std::cout << url << std::endl;
    }

    using namespace std::placeholders;
    using namespace curlpp;
    using namespace curlpp::options;


    Cleanup cleaner;
    Easy request;

    auto* fn = new WriteFunction( std::bind( &WriteToStream, std::ref( out ), _1, _2, _3 ) );
    request.setOpt( fn );
    request.setOpt( new Url( url ) );
    request.setOpt( new Verbose( client_settings_->verbose_ ) );
    request.setOpt( new HttpHeader( client_settings_->http_headers_ ) );

    // TODO: CA check to be fixed properly. Just for testing purposes.
    request.setOpt( SslVerifyPeer( false ) );
    request.setOpt( SslVerifyHost( false ) );

    request.perform( );

    const auto http_code = curlpp::infos::ResponseCode::get( request );
    if ( http_code < 200 || http_code >= 300 )
    {
        // curlpp does no throw an exception when HTTP code is not OK.
        std::cerr << "HTTP error code: " << http_code << std::endl;
        throw download::HttpRuntimeError( "Http error", http_code );
    }
}

void HttpClient::GetUri( const std::string& url, rapidjson::Document& out ) const
{
    std::stringstream ss;
    GetUri( url, ss );
    out.Parse( ss.str( ).c_str( ) );
}

} // namespace download
} // namespace map_service
