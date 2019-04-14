project(stb-image)

# Define static library (only .cpp)
add_library(${PROJECT_NAME} STATIC src/stb_image.cpp)

# Create nice looking alias for clients of bge to link to 
add_library(stb::image ALIAS ${PROJECT_NAME})

# Set include directories
target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    PRIVATE
        ${PROJECT_SOURCE_DIR}/src
)