/*
 * SPDX-FileCopyrightText: Copyright DB Netz AG
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ProtobufCleaner.h"
#include "org/db/schema/rca/topology/v2/base_layer.pb.h"

namespace map_service
{
namespace utils
{
ProtobufCleaner::ProtobufCleaner( )
{
    GOOGLE_PROTOBUF_VERIFY_VERSION;
}

ProtobufCleaner::~ProtobufCleaner( )
{
    google::protobuf::ShutdownProtobufLibrary( );
}

} // namespace utils
} // namespace map_service
