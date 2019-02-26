project(sandbox)

# Define executable (.cpp only)
add_executable(${PROJECT_NAME} src/SandboxApp.cpp)

# Set Output dir of library to be in build/bin
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Set include directories
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/src)

# Compiler standard
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_14)

# Link libraries
target_link_libraries(${PROJECT_NAME} PUBLIC bge::bge)