project(tinyobjloader)

# Define static library (only .cpp)
add_library(${PROJECT_NAME} STATIC src/tiny_obj_loader.cpp)

# Create nice looking alias for clients of bge to link to 
add_library(tinyobj::tinyobj ALIAS ${PROJECT_NAME})

# Set include directories
target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    PRIVATE
        ${PROJECT_SOURCE_DIR}/src
)