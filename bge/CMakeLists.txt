project(bge-engine)

# Define shared library (only .cpp)
add_library(${PROJECT_NAME} STATIC 
  src/core/Application.cpp

  src/ecs/ComponentTraits.cpp
  src/ecs/EntityManager.cpp
  src/ecs/World.cpp

  src/input/UnixInput.cpp

  src/logging/Log.cpp

  src/math/AABB.cpp
  src/math/Transform.cpp

  src/physics/BoxSystem.cpp
  src/physics/PhysicsDevice.cpp
  src/physics/PhysicsWorld.cpp
  src/physics/SphereSystem.cpp

  src/rendering/CameraManager.cpp
  src/rendering/MeshLibrary.cpp
  src/rendering/MeshSystem.cpp
  src/rendering/OpenGLRenderDevice.cpp
  src/rendering/PhysicalMeshSystem.cpp
  src/rendering/RenderWorld.cpp
  src/rendering/ShaderLibrary.cpp
  src/rendering/Texture2DLibrary.cpp

  src/scheduler/Scheduler.cpp
  src/scheduler/Task.cpp
  src/scheduler/WorkStealingQueue.cpp

  src/util/FileIO.cpp
  src/util/RandomNumberGenerator.cpp
  
  src/video/UnixWindow.cpp)

# Create nice looking alias for clients of bge to link to 
add_library(bge::bge ALIAS ${PROJECT_NAME})

# Platform specific preprocessor defines
# PUBLIC  = define is visible to the user of bge
# PRIVATE = define is visible to bge only
if (WIN32)
    target_compile_definitions(${PROJECT_NAME} PUBLIC BGE_PLATFORM_WINDOWS=1)
endif()
if(APPLE)
    # for MacOS X or iOS, watchOS, tvOS (since 3.10.3)
    target_compile_definitions(${PROJECT_NAME} PUBLIC BGE_PLATFORM_APPLE=1)
endif()
if(UNIX AND NOT APPLE)
    # for Linux, BSD, Solaris, Minix
    target_compile_definitions(${PROJECT_NAME} PUBLIC BGE_PLATFORM_UNIX=1)
endif()

#platform independant preprocessor defines
target_compile_definitions(${PROJECT_NAME} PRIVATE BGE_BUILD_SHARED=1)
target_compile_definitions(${PROJECT_NAME} PRIVATE GLFW_INCLUDE_NONE=1)

target_compile_options(${PROJECT_NAME} PRIVATE
    $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:GNU>>:
        -pipe -march=native
        $<$<CONFIG:Debug>:-O0 -Wall -Wextra>>
    $<$<CXX_COMPILER_ID:MSVC>:
        $<$<CONFIG:Debug>:/Od /Wall /Zi>>
)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  target_compile_definitions(${PROJECT_NAME} PUBLIC BGE_DEBUG=1)
endif()

# Set include directories
target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${PROJECT_SOURCE_DIR}/src
)

# Language standard
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_14)

# Add external libraries

# spdlog
add_subdirectory(contrib/spdlog)

# glfw
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(contrib/glfw)

# glad
add_subdirectory(contrib/glad)

# stb image
add_subdirectory(contrib/stb)

# tiny obj loader
add_subdirectory(contrib/tinyobj)

# nudge
add_subdirectory(contrib/nudge)

# Link libraries (e.g. logging)
target_link_libraries(${PROJECT_NAME} 
    ${CMAKE_THREAD_LIBS_INIT}     # Needed for unix builds to link to pthread
    spdlog::spdlog
    glfw
    glad::glad
    stb::image
    tinyobj::tinyobj
    nudge::nudge
)