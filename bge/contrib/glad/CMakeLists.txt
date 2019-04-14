project(glad LANGUAGES C)

# Define static library (only .c)
add_library(${PROJECT_NAME} STATIC src/glad.c)

# Create nice looking alias for clients of bge to link to 
add_library(glad::glad ALIAS ${PROJECT_NAME})

# Set Properties
set_target_properties(${PROJECT_NAME} PROPERTIES
                      POSITION_INDEPENDENT_CODE ON)

# Set include directories
target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    PRIVATE
        ${PROJECT_SOURCE_DIR}/src
)