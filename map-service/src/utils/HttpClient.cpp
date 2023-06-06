#include <utils/HttpClient.h>
#include <map-service/download/ClientSettings.h>

#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>

#include <sstream>

namespace map_service
{
namespace utils
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

download::Error
HttpClient::GetUri( const std::string& url, std::ostream& out ) const
{
    // if ( client_settings_->verbose_ )
    {
        std::cout << url << std::endl;
    }

    using namespace std::placeholders;
    using namespace curlpp;
    using namespace curlpp::options;

    try
    {
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
        return Error( );
    }
    catch ( const LibcurlRuntimeError& e )
    {
        // TODO: curl thorws RuntimeError when incorrect host is specified
        // or when 404 is returned.
        // CURL code is not provided in this case.
        // Handle RuntimeError separatly.
        std::cerr << e.what( ) << std::endl;

        return { ErrorCode::CurlError, e.whatCode( ), url, e.what( ) };
    }
    catch( const std::exception& e)
    {
        std::cerr << e.what( ) << std::endl;
        throw;
    }
    catch(...)
    {
        std::cerr << "Http client unknown exception" << std::endl;
        throw;
    }
}

download::Error
HttpClient::GetUri( const std::string& url, rapidjson::Document& out ) const
{
    std::stringstream ss;

    const auto error = GetUri( url, ss );
    if ( error.error_code_ == ErrorCode::Success )
    {

        out.Parse( ss.str( ).c_str( ) );
    }
    return error;
}

} // namespace utils
} // namespace map_service
