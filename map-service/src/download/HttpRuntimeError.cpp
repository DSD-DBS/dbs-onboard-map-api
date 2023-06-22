/*
 * SPDX-FileCopyrightText: Copyright DB Netz AG
 * SPDX-License-Identifier: Apache-2.0
 */

#include <map-service/download/HttpRuntimeError.h>

namespace map_service
{
namespace download
{
HttpRuntimeError::HttpRuntimeError( const std::string& reason, HttpCode code )
: std::runtime_error( reason ), code_( code )
{
}

HttpRuntimeError::HttpRuntimeError( const char* reason, HttpCode code )
: std::runtime_error( reason ), code_( code )
{
}

HttpRuntimeError::HttpCode
HttpRuntimeError::WhatCode( ) const throw( )
{
    return code_;
}

} // namespace download
} // namespace map_service
