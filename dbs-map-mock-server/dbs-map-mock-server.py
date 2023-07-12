# SPDX-FileCopyrightText: Copyright DB Netz AG
# SPDX-License-Identifier: CC0-1.0

import http.server
import socketserver
import os
import re

class CustomHttpRequestHandler(http.server.SimpleHTTPRequestHandler):
    def do_GET(self):
        print("GET: ", self.path)
        get_catalog_metadata_pattern = r"^/catalogs/(?P<catalogId>[^/]+)(\?catalogVersion=(?P<catalogVersion>[^&]+))?"
        get_catalog_metadata_uri = "./hdmap/{}/current/metadata.json"
        # get_catalog_metadata_with_version_uri = f"./hdmap/catalogs/{catalogId}"

        match = re.match(get_catalog_metadata_pattern, self.path)
        if match:
            print(match.groupdict())
            params = match.groupdict()
            print(params['catalogId'])
            self.path = get_catalog_metadata_uri.format(params['catalogId'])
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
