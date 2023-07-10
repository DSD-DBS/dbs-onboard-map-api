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

/**
 * @class HttpRuntimeError
 * @brief Custom exception class for handling HTTP errors.
 *
 * This class extends from std::runtime_error and is thrown when an HTTP error occurs.
 * Specifically, it is thrown when the HTTP status code is 300 or greater. It includes
 * methods for providing the reason and HTTP status code of the error.
 */
class HttpRuntimeError : public std::runtime_error
{
public:
    using HttpCode = int;

    /**
     * @brief Constructs an HttpRuntimeError object with a specific reason and HTTP status code.
     *
     * @param[in] reason The reason for the HTTP error.
     * @param[in] code The HTTP status code.
     */
    HttpRuntimeError( const std::string& reason, HttpCode code );

    /**
     * @brief Constructs an HttpRuntimeError object with a specific reason and HTTP status code.
     *
     * @param[in] reason The reason for the HTTP error.
     * @param[in] code The HTTP status code.
     */
    HttpRuntimeError( const char* reason, HttpCode code );

    /**
     * @brief Retrieves the HTTP status code associated with the error.
     *
     * This method is used to retrieve the HTTP status code that is associated with the error.
     * The code can then be used to determine the specific type of error that occurred.
     *
     * @return The HTTP status code associated with the error.
     */
    HttpCode WhatCode( ) const throw( );

private:
    HttpCode code_;
};


} // namespace download
} // namespace map_service
