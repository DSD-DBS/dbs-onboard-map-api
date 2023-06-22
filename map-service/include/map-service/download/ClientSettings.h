
/*
 * SPDX-FileCopyrightText: Copyright DB Netz AG
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#include <string>
#include <list>

namespace map_service
{
namespace download
{

struct ClientSettings
{
    std::string host_;
    std::list< std::string > http_headers_;
    bool verbose_ = false;
};

ClientSettings GetDBClientSettings( );
ClientSettings GetDBClientForCISettings( );

} // namespace download
} // namespace map_service
