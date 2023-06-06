#pragma once
#include <curl/curl.h>
#include <string>

namespace map_service
{
namespace download
{

enum ErrorCode
{
    Unknown = 0,
    Success,
    ContentSizeError,
    ChecksumError,
    CurlError, // Check CURLCode for more details
};

struct Error
{
    ErrorCode error_code_ = ErrorCode::Success;
    CURLcode curl_code_ = CURLE_OK;
    std::string uri_;
    std::string msg_;
};

template < class TContent >
struct Response
{
    using Content = TContent;

    Error error_;
    Content content_;
};

} // namespace download
} // namespace map_service
