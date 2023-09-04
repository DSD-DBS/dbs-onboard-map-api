cmake_minimum_required(VERSION 3.10)

project(StructurizrIntegration)

# Define the output directory for PlantUML files
set(STRUCTURIZR_OUTPUT_PATH "${CMAKE_BINARY_DIR}/structurizr-output")
set(STRUCTURIZR_OUTPUT_FORMAT "ilograph")
# plantuml
set(STRUCTURIZR_CLI_VERSION "1.33.1") 
set(STRUCTURIZR_CLI_ZIP_URL "https://github.com/structurizr/cli/releases/download/v${STRUCTURIZR_CLI_VERSION}/structurizr-cli-${STRUCTURIZR_CLI_VERSION}.zip")
set(STRUCTURIZR_CLI_ZIP_PATH "${CMAKE_BINARY_DIR}/structurizr-cli-${STRUCTURIZR_CLI_VERSION}.zip")
# set(STRUCTURIZR_CLI_JAR_PATH "${CMAKE_BINARY_DIR}/structurizr-cli-${STRUCTURIZR_CLI_VERSION}.jar")
set(STRUCTURIZR_CLI_SH_PATH "${CMAKE_BINARY_DIR}/structurizr.sh")


# Download structurizr-cli.zip if it doesn't exist
if(NOT EXISTS ${STRUCTURIZR_CLI_ZIP_PATH})
    message(STATUS "Downloading Structurizr CLI ZIP v${STRUCTURIZR_CLI_VERSION}")

    file(DOWNLOAD ${STRUCTURIZR_CLI_ZIP_URL} ${STRUCTURIZR_CLI_ZIP_PATH}
         SHOW_PROGRESS
         STATUS status
         LOG log)

    list(GET status 0 error_code)
    list(GET status 1 error_msg)

    if(error_code)
        message(FATAL_ERROR "Error downloading Structurizr CLI ZIP: ${error_msg}\nLog: ${log}")
    endif()

    message(STATUS "Structurizr CLI ZIP downloaded to ${STRUCTURIZR_CLI_ZIP_PATH}")
endif()

# Extract the ZIP if the JAR file doesn't exist
if(NOT EXISTS ${STRUCTURIZR_CLI_JAR_PATH})
    message(STATUS "Extracting Structurizr CLI ZIP to retrieve the JAR")

    file(ARCHIVE_EXTRACT
         INPUT ${STRUCTURIZR_CLI_ZIP_PATH}
         DESTINATION ${CMAKE_BINARY_DIR}
         )

    message(STATUS "Structurizr CLI JAR extracted to ${STRUCTURIZR_CLI_JAR_PATH}")
endif()

message("outout: ${STRUCTURIZR_OUTPUT_PATH}")
message("input: ${CMAKE_CURRENT_SOURCE_DIR}/your_model.dsl")
# Add custom target to generate diagrams using structurizr-cli.jar
add_custom_target(structurizr
    COMMAND ${CMAKE_COMMAND} -E echo "Generating diagrams using Structurizr..."
    # COMMAND java -jar ${STRUCTURIZR_CLI_JAR_PATH} export -workspace ${CMAKE_CURRENT_SOURCE_DIR}/your_model.dsl -format plantuml -output ${STRUCTURIZR_OUTPUT_PATH}
    COMMAND ${STRUCTURIZR_CLI_SH_PATH} export -workspace ${CMAKE_CURRENT_SOURCE_DIR}/your_model.dsl -format ${STRUCTURIZR_OUTPUT_FORMAT} -output ${STRUCTURIZR_OUTPUT_PATH}
    COMMAND ${CMAKE_COMMAND} -E echo "Diagrams generated and saved to ${STRUCTURIZR_OUTPUT_PATH}."
)