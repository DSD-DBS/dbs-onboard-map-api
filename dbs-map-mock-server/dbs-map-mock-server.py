# SPDX-FileCopyrightText: Copyright DB Netz AG
# SPDX-License-Identifier: CC0-1.0

import http.server
import socketserver
import os
import re

class CustomHttpRequestHandler(http.server.SimpleHTTPRequestHandler):
    def do_GET(self):
        print("GET: ", self.path)
        get_catalog_metadata_pattern = r"^/catalogs/(?P<catalogId>[^/?]+)$"
        get_catalog_metadata_path = "./hdmap/{}/current/metadata.json"

        get_catalog_metadata_with_version_pattern = r"^/catalogs/(?P<catalogId>[^/?]+)\?catalogVersion=(?P<catalogVersion>\d+)"
        get_catalog_metadata_with_version_path = "./hdmap/{}/{}/metadata.json"

        # match = re.match(get_catalog_metadata_pattern, self.path)
        match = re.search(get_catalog_metadata_pattern, self.path)
        if match:
            catalogId = match.group('catalogId')
            self.path = get_catalog_metadata_path.format(catalogId)

        match = re.search(get_catalog_metadata_with_version_pattern, self.path)
        if match:
            catalogId = match.group('catalogId')
            catalogVersion = match.group('catalogVersion')
            self.path = get_catalog_metadata_with_version_path.format(catalogId, catalogVersion)

        print(self.path)

        # match_get_layer_metadata = ""
        # match_get_partition_metadata = ""
        # match_get_partition_data = ""
        # match_get_all_partition_match = ""

        return http.server.SimpleHTTPRequestHandler.do_GET(self)

Handler = CustomHttpRequestHandler

with socketserver.TCPServer(("localhost", 8080), Handler) as httpd:
    print("serving at port", 8080)
    httpd.serve_forever()
