/*
 * SPDX-FileCopyrightText: Copyright DB Netz AG
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#include <curl/curl.h>
#include <string>
#include <stdexcept>

namespace map_service
{
namespace download
{

class HttpRuntimeError : public std::runtime_error
{
public:
    using HttpCode = int;

    HttpRuntimeError( const std::string& reason, HttpCode code );
    HttpRuntimeError( const char* reason, HttpCode code );

    HttpCode WhatCode( ) const throw( );

private:
    HttpCode code_;
};

} // namespace download
} // namespace map_service
