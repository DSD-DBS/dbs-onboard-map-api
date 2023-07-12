# SPDX-FileCopyrightText: Copyright DB Netz AG
# SPDX-License-Identifier: CC0-1.0

import http.server
import socketserver
import os
import re

def get_catalog_matadata(match):
    path = "./hdmap/{}/current/metadata.json"
    catalogId = match.group('catalogId')
    return path.format(catalogId)

def get_catalog_matadata_with_version(match):
    path = "./hdmap/{}/{}/metadata.json"
    catalogId = match.group('catalogId')
    catalogVersion = match.group('catalogVersion')
    return path.format(catalogId, catalogVersion)

def get_layer_matadata(match):
    path = "./hdmap/{}/current/{}/metadata.json"
    catalogId = match.group('catalogId')
    layerId = match.group('layerId')
    return path.format(catalogId, layerId)

def get_layer_matadata_with_version(match):
    path = "./hdmap/{}/{}/{}/metadata.json"
    catalogId = match.group('catalogId')
    layerId = match.group('layerId')
    catalogVersion = match.group('catalogVersion')
    return path.format(catalogId, catalogVersion, layerId)

# def get_layer_partitions(match):
#     path = "./hdmap/{}/current/{}/partitions/{}_metadata.json"
#     catalogId = match.group('catalogId')
#     layerId = match.group('layerId')
#     partitionId = match.group('partitionId')
#     return path.format(catalogId, layerId, partitionId)

# def get_layer_partitions_with_version(match):
#     path = "./hdmap/{}/{}/{}/partitions/{}_metadata.json"
#     catalogId = match.group('catalogId')
#     layerId = match.group('layerId')
#     partitionId = match.group('partitionId')
#     catalogVersion = match.group('catalogVersion')
#     return path.format(catalogId, catalogVersion, layerId, partitionId)

def get_partition_metadata(match):
    path = "./hdmap/{}/current/{}/partitions/{}_metadata.json"
    catalogId = match.group('catalogId')
    layerId = match.group('layerId')
    partitionId = match.group('partitionId')
    return path.format(catalogId, layerId, partitionId)

def get_partition_metadata_with_version(match):
    path = "./hdmap/{}/{}/{}/partitions/{}_metadata.json"
    catalogId = match.group('catalogId')
    layerId = match.group('layerId')
    partitionId = match.group('partitionId')
    catalogVersion = match.group('catalogVersion')
    return path.format(catalogId, catalogVersion, layerId, partitionId)

def get_partition_data_with_version(match):
    path = "./hdmap/{}/{}/{}/partitions/{}"
    catalogId = match.group('catalogId')
    layerId = match.group('layerId')
    partitionId = match.group('partitionId')
    catalogVersion = match.group('catalogVersion')
    return path.format(catalogId, catalogVersion, layerId, partitionId)

def match_to_path(self):
    # Match to GET /catalogs/{catalogId}
    pattern = r"^/catalogs/(?P<catalogId>[^/?]+)$"
    match = re.search(pattern, self.path)
    if match:
        return get_catalog_matadata(match)

    # Match to GET /catalogs/{catalogId}?catalogVersion=
    pattern = r"^/catalogs/(?P<catalogId>[^/?]+)\?catalogVersion=(?P<catalogVersion>\d+)"
    match = re.search(pattern, self.path)
    if match:
        return get_catalog_matadata_with_version(match)

    # Match to GET /catalogs/{catalogId}/layers/{layerId}
    pattern = r"/catalogs/(?P<catalogId>[^/]+)/layers/(?P<layerId>[^/]+)"
    match = re.search(pattern, self.path)
    if match:
        return get_layer_matadata(match)

    # Match to GET /catalogs/{catalogId}/layers/{layerId}?catalogVersion=
    pattern = r"/catalogs/(?P<catalogId>[^/]+)/layers/(?P<layerId>[^/]+)\?catalogVersion=(?P<catalogVersion>\d+)"
    match = re.search(pattern, self.path)
    if match:
        return get_layer_matadata_with_version(match)

    # Match to /catalogs/{catalogId}/layers/{layerId}/blobs
    # pattern = r"/catalogs/(?P<catalogId>[^/]+)/layers/(?P<layerId>[^/]+)/blobs"
    # match = re.search(pattern, self.path)
    #     if match:
    #         return get_layer_partitions(match)

    # Match to /catalogs/{catalogId}/layers/{layerId}/blobs?catalogVersion=
    # pattern = r"/catalogs/(?P<catalogId>[^/]+)/layers/(?P<layerId>[^/]+)/blobs\?catalogVersion=(?P<catalogVersion>\d+)"
    # match = re.search(pattern, self.path)
    #     if match:
    #         return get_layer_partitions_with_version(match)

    # Match to /catalogs/{catalogId}/layers/{layerId}/blobs/{blobKey}
    pattern = r"/catalogs/(?P<catalogId>[^/]+)/layers/(?P<layerId>[^/]+)/blobs/(?P<blobKey>[^/]+)"
    match = re.search(pattern, self.path)
    if match:
        return get_partition_matadata(match)

    # Match to /catalogs/{catalogId}/layers/{layerId}/blobs/{blobKey}?catalogVersion=
    pattern = r"/catalogs/(?P<catalogId>[^/]+)/layers/(?P<layerId>[^/]+)/blobs/(?P<blobKey>[^/]+)\?catalogVersion=(?P<catalogVersion>\d+)"
    match = re.search(pattern, self.path)
    if match:
        return get_partition_matadata_with_version(match)

    # Match to /blob/{catalogId}/{catalogVersion}/{layerId}/{blobKey}
    pattern = r"/blob/(?P<catalogId>[^/]+)/(?P<catalogVersion>\d+)/(?P<layerId>[^/]+)/(?P<blobKey>[^/]+)"
    match = re.search(pattern, self.path)
    if match:
        return get_partition_data_with_version(match)

class CustomHttpRequestHandler(http.server.SimpleHTTPRequestHandler):

    def do_GET(self):
        print("GET: ", self.path)
        file_path = match_to_path(self)
        if file_path:
            self.path = file_path
            print("Path to file: ", self.path)

        return http.server.SimpleHTTPRequestHandler.do_GET(self)

Handler = CustomHttpRequestHandler

with socketserver.TCPServer(("localhost", 8080), Handler) as httpd:
    print("serving at port", 8080)
    httpd.serve_forever()
