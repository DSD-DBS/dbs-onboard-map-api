# SPDX-FileCopyrightText: Copyright DB Netz AG
# SPDX-License-Identifier: Apache-2.0

import http.server
import json
import os
import re
import socketserver


def find_blobkey_by_datahandle(directory, data_handle):
    for root, dirs, files in os.walk(directory):
        for file in files:
            if file.endswith('.json'):
                filepath = os.path.join(root, file)
                with open(filepath, 'r') as json_file:
                    try:
                        data = json.load(json_file)
                        if 'dataHandle' in data and data['dataHandle'] == data_handle:
                            return data.get('blobKey')
                    except json.JSONDecodeError:
                        print(f"File {filepath} is not a valid JSON file.")
    return None

def get_catalog_matadata(match):
    path = './hdmap/{}/{}/metadata.json'
    return path.format(
        match.group('catalogId'),
        match.groupdict().get('catalogVersion', 'current'))

def get_layer_matadata(match):
    path = "./hdmap/{}/{}/{}/metadata.json"
    return path.format(
        match.group('catalogId'),
        match.groupdict().get('catalogVersion', 'current'),
        match.group('layerId'))

def get_all_blobs_metadata(match):
    path = "./hdmap/{}/{}/{}/blobs.json"
    return path.format(
        match.group('catalogId'),
        match.groupdict().get('catalogVersion', 'current'),
        match.group('layerId'))

def get_partition_metadata(match):
    path = "./hdmap/{}/{}/{}/partitions/{}_metadata.json"
    return path.format(
        match.group('catalogId'),
        match.groupdict().get('catalogVersion', 'current'),
        match.group('layerId'),
        match.group('blobKey'))

def get_partition_data(match):
    path = "./hdmap/{}/{}/{}/partitions/"

    partitions_folder = path.format(
        match.group('catalogId'),
        match.groupdict().get('catalogVersion', 'current'),
        match.group('layerId'))

    file_name = find_blobkey_by_datahandle( partitions_folder, match.group('dataHandle') )
    return partitions_folder + file_name if file_name else None


# /catalogs/{catalogId}
# /catalogs/{catalogId}/layers/{layerId}
# /catalogs/{catalogId}/layers/{layerId}/blobs
# /catalogs/{catalogId}/layers/{layerId}/blobs/{blobKey}
# /blob/catalogs/{catalogId}/layers/{layerId}/data/{dataHandle}

# regex pattern to match optional catalogVersion query parameter
optional_version = r"(?:\?catalogVersion=\d+)?$"

# array of regex patterns and handlers
route_handlers = [
    # /catalogs/{catalogId}
    (re.compile(rf"^/catalogs/(?P<catalogId>[^/?]+){optional_version}"), get_catalog_matadata),

    # /catalogs/{catalogId}/layers/{layerId}
    (re.compile(rf"^/catalogs/(?P<catalogId>[^/?]+)/layers/(?P<layerId>[^/?]+){optional_version}"), get_layer_matadata),

    # /catalogs/{catalogId}/layers/{layerId}/blobs
    (re.compile(rf"^/catalogs/(?P<catalogId>[^/?]+)/layers/(?P<layerId>[^/?]+)/blobs{optional_version}"), get_all_blobs_metadata),

    # /catalogs/{catalogId}/layers/{layerId}/blobs/{blobKey}
    (re.compile(rf"^/catalogs/(?P<catalogId>[^/?]+)/layers/(?P<layerId>[^/?]+)/blobs/(?P<blobKey>[^/?]+){optional_version}"), get_partition_metadata),

    # /blob/catalogs{catalogId}/layers/{layerId}/data/{dataHandle}
    (re.compile(rf"^/blob/catalogs/(?P<catalogId>[^/?]+)/layers/(?P<layerId>[^/?]+)/data/(?P<dataHandle>[^/?]+){optional_version}"), get_partition_data)
]

def route_to_file(self):
    for pattern, handler in route_handlers:
        match = pattern.match(self.path)
        print(pattern)
        if match:
            print('matched')
            return handler(match)
    return None

class CustomHttpRequestHandler(http.server.SimpleHTTPRequestHandler):
    def do_GET(self):
        print("GET: ", self.path)
        file_path = route_to_file(self)
        if file_path:
            self.path = file_path
            print("Path to file: ", self.path)

        return http.server.SimpleHTTPRequestHandler.do_GET(self)

Handler = CustomHttpRequestHandler

with socketserver.TCPServer(("localhost", 8080), Handler) as httpd:
    print("serving at port", 8080)
    httpd.serve_forever()
