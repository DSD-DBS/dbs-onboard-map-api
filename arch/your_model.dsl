workspace {

    model {
        cloud = softwareSystem "DB cloud" {
            mapGen = container "Map Generation pipeline" {
            }
            mapCDB = container "Map Central Database" { 
                mapCDB -> mapGen "Uses"
            }
        } 
        train = softwareSystem "Train" {
            mapApi = container "DBS Onboard Map API" {
                config = component "Configuration" {
                }
                decoderLandmark = component "Landmark Decoder" {
                }
                decoderTopology = component "Topology Decoder" {
                }
                decoderRAZ = component "Risk Assesment Zones Decoder" {
                }
                decoderGeometry = component "Track Geometry Decoder" {
                }
                mapUpdater = component "Map Updater" {
                }
                mapServiceImpl = component "Map Service Implementation" {
                }
                mapService = component "Map Service" {
                }
                mapService -> mapServiceImpl
                
                mapServiceImpl -> config
                mapServiceImpl -> decoderTopology
                mapServiceImpl -> decoderGeometry
                mapServiceImpl -> decoderLandmark
                mapServiceImpl -> decoderRAZ
                mapServiceImpl -> mapUpdater
            }
            railHorizon = container "Rail Horizon" {
                railHorizon -> mapApi "Uses"
            }
            sensors = container "Sensors" {
                sensors -> railHorizon "Uses"
            }
            train -> cloud "Uses"
            mapApi -> mapCDB "Fetch map data from DB cloud"
        }
    }

    views {
        // systemContext train {
        //     include cloud
        //     include train
        //     autolayout lr
        // }

        // container train {
        //     include *
        //     autolayout lr
        // }

        // container mapApi {
        //     include *
        //     autolayout lr
        // }

        // systemContext softwareSystem {
        //     include *
        //     autolayout lr
        // }

        // container softwareSystem {
        //     include *
        //     autolayout lr
        // }

        theme default
    }
}
