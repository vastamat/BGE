project(dod-examples)

# Define executable (.cpp only)
add_executable(${PROJECT_NAME} main.cpp)

# Set Output dir of library to be in build/bin
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Compiler standard
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_14)

# Link libraries
target_link_libraries(${PROJECT_NAME} PUBLIC bge::bge)