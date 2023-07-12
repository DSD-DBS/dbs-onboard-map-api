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
