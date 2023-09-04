workspace {

    model {
        user = person "User"
        softwareSystem = softwareSystem "Shopping System" {
            // description = "Allows users to browse and purchase products"
            
            webapp = container "Web Application" {
                // description = "Serves product info and processes purchases"
                // technology = "Web Application"
                // tags = "App"
                
                user -> this "Uses"
            }
            
            database = container "Database" {
                // description = "Stores product inventory and purchase records"
                // technology = "Relational Database"
                // tags = "Database"
                
                webapp -> this "Reads from and writes to"
            }
        }
    }

    views {
        systemContext softwareSystem {
            include *
            autolayout lr
        }

        container softwareSystem {
            include *
            autolayout lr
        }

        theme default
    }
}
