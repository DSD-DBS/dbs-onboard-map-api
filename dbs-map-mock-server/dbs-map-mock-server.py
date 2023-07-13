# SPDX-FileCopyrightText: Copyright DB Netz AG
# SPDX-License-Identifier: Apache-2.0

import json
import os

from flask import Flask, request, send_file

app = Flask(__name__)

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
    path = "./hdmap/{}/{}/{}/partitions/"

    partitions_folder = path.format(catalog_id, version(request), layer_id)

    file_name = find_blobkey_by_datahandle( partitions_folder, data_handle)
    return send_file(partitions_folder + file_name if file_name else None)

# /catalogs/{catalogId}
# /catalogs/{catalogId}/layers/{layerId}
# /catalogs/{catalogId}/layers/{layerId}/blobs
# /catalogs/{catalogId}/layers/{layerId}/blobs/{blobKey}
# /blob/catalogs/{catalogId}/layers/{layerId}/data/{dataHandle}


if __name__ == '__main__':
    app.run(port=8080)
