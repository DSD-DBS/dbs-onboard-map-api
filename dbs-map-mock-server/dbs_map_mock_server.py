# SPDX-FileCopyrightText: Copyright DB Netz AG
# SPDX-License-Identifier: Apache-2.0

"""dbs_map_mock_server.py This module mocks DB Map server REST API.

Map data and metadata is expected to be stored locallys.
"""

from flask import Flask, request, send_file

app = Flask(__name__)

def version(req):
    """Retrieve the 'catalogVersion' from the request arguments.

    Returns 'current' if 'catalogVersion' is not specified.
    """
    return req.args.get('catalogVersion', 'current')


@app.route('/catalogs/<catalog_id>', methods=['GET'])
def get_catalog_matadata(catalog_id):
    """Get metadata for a specified catalog.

    The metadata file is expected to be in the
    'hdmap/{catalogId}/{version}' directory.
    """
    path = './hdmap/{}/{}/metadata.json'
    return send_file(path.format(catalog_id, version(request)))


@app.route('/catalogs/<catalog_id>/layers/<layer_id>', methods=['GET'])
def get_layer_matadata(catalog_id, layer_id):
    """Get metadata for a specified layer in a catalog.

    The metadata file is expected to be in the
    'hdmap/{catalogId}/{version}/{layerId}' directory.
    """
    path = "./hdmap/{}/{}/{}/metadata.json"
    return send_file(path.format(catalog_id, version(request), layer_id))


@app.route('/catalogs/<catalog_id>/layers/<layer_id>/blobs', methods=['GET'])
def get_all_blobs_metadata(catalog_id, layer_id):
    """Get metadata for all blobs in a specified layer in a catalog.

    The metadata file is expected to be in the
    'hdmap/{catalogId}/{version}/{layerId}' directory.
    """
    path = "./hdmap/{}/{}/{}/blobs.json"
    return send_file(path.format(catalog_id, version(request), layer_id))


@app.route('/catalogs/<catalog_id>/layers/<layer_id>/blobs/<blob_key>', methods=['GET'])
def get_partition_metadata(catalog_id, layer_id, blob_key):
    """Get metadata for a specified partition (blob) in a layer in a catalog.

    The metadata file is expected to be in the
    'hdmap/{catalogId}/{version}/{layerId}/partitions' directory.
    """
    path = "./hdmap/{}/{}/{}/partitions/{}_metadata.json"
    return send_file(path.format(catalog_id, version(request), layer_id, blob_key))


@app.route('/blob/catalogs/<catalog_id>/layers/<layer_id>/data/<data_handle>', methods=['GET'])
def get_partition_data(catalog_id, layer_id, data_handle):
    """Get data for a specified partition in a layer in a catalog."""
    path = "./hdmap/{}/{}/{}/partitions/{}"
    return send_file(path.format(catalog_id, version(request), layer_id, data_handle))

if __name__ == '__main__':
    app.run()
