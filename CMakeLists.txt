cmake_minimum_required(VERSION 3.8)

# Project
project(bge)

# Options
option(BGE_BUILD_SANDBOX "Build sandbox application" ON)
option(BGE_BUILD_DOD_EXAMPLES "Build dod examples" ON)

# engine
add_subdirectory(bge)

# sandbox
if(BGE_BUILD_SANDBOX)
    add_subdirectory(sandbox)
endif()
if(BGE_BUILD_DOD_EXAMPLES)
    add_subdirectory(dod-examples)
endif()