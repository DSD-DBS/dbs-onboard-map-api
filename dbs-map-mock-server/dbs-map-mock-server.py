# SPDX-FileCopyrightText: Copyright DB Netz AG
# SPDX-License-Identifier: Apache-2.0

import json
import os

from flask import Flask, request, send_file

app = Flask(__name__)

def version(request):
    return request.args.get('catalogVersion', 'current')

@app.route('/catalogs/<catalog_id>', methods=['GET'])
def get_catalog_matadata(catalog_id):
    path = './hdmap/{}/{}/metadata.json'
    return send_file(path.format(catalog_id, version(request)))

@app.route('/catalogs/<catalog_id>/layers/<layer_id>', methods=['GET'])
def get_layer_matadata(catalog_id, layer_id):
    path = "./hdmap/{}/{}/{}/metadata.json"
    return send_file(path.format(catalog_id, version(request), layer_id))

@app.route('/catalogs/<catalog_id>/layers/<layer_id>/blobs', methods=['GET'])
def get_all_blobs_metadata(catalog_id, layer_id):
    path = "./hdmap/{}/{}/{}/blobs.json"
    return send_file(path.format(catalog_id, version(request), layer_id))

@app.route('/catalogs/<catalog_id>/layers/<layer_id>/blobs/<blob_key>', methods=['GET'])
def get_partition_metadata(catalog_id, layer_id, blob_key):
    path = "./hdmap/{}/{}/{}/partitions/{}_metadata.json"
    return send_file(path.format(catalog_id, version(request), layer_id, blob_key))

@app.route('/blob/catalogs/<catalog_id>/layers/<layer_id>/data/<data_handle>', methods=['GET'])
def get_partition_data(catalog_id, layer_id, data_handle):
    path = "./hdmap/{}/{}/{}/partitions/{}"
    return send_file(path.format(catalog_id, version(request), layer_id, data_handle))

# /catalogs/{catalogId}
# /catalogs/{catalogId}/layers/{layerId}
# /catalogs/{catalogId}/layers/{layerId}/blobs
# /catalogs/{catalogId}/layers/{layerId}/blobs/{blobKey}
# /blob/catalogs/{catalogId}/layers/{layerId}/data/{dataHandle}


if __name__ == '__main__':
    app.run(port=8080)
