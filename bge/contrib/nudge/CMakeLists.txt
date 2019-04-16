project(nudge)

# Define static library (only .cpp)
add_library(${PROJECT_NAME} STATIC src/nudge.cpp)

# Create nice looking alias for clients of bge to link to 
add_library(nudge::nudge ALIAS ${PROJECT_NAME})

# Set include directories
target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    PRIVATE
        ${PROJECT_SOURCE_DIR}/src
)