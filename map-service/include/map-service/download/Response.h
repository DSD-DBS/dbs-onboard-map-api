/*
 * SPDX-FileCopyrightText: Copyright DB Netz AG
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#include <curl/curl.h>
#include <string>

namespace map_service
{
namespace download
{
template < class TContent >
struct Response
{
    using Content = TContent;
    Content content_;
};

} // namespace download
} // namespace map_service
