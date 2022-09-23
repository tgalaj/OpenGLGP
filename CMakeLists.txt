cmake_minimum_required(VERSION 3.21 FATAL_ERROR)
project(OpenGLPAG VERSION 2.0)

set(THIRDPARTY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty)

# ---- Dependencies ----
include(cmake/CPM.cmake)

# OpenGL
find_package(OpenGL REQUIRED)
set(OPENGL_LIBRARY ${OPENGL_LIBRARIES})

# glad
set(GLAD_DIR ${THIRDPARTY_DIR}/glad)
add_library(glad STATIC ${GLAD_DIR}/src/glad.c)
target_include_directories(glad PRIVATE ${GLAD_DIR}/include)

set(GLAD_LIBRARY glad)
set(GLAD_INCLUDE_DIR ${GLAD_DIR}/include)

# stb_image
set(STB_IMAGE_DIR ${THIRDPARTY_DIR}/stb_image)
add_library(stb_image STATIC ${STB_IMAGE_DIR}/stb_image.cpp)
target_include_directories(stb_image PRIVATE ${STB_IMAGE_DIR})

set(STB_IMAGE_LIBRARY stb_image)
set(STB_IMAGE_INCLUDE_DIR ${STB_IMAGE_DIR})

# other
CPMAddPackage("gh:assimp/assimp@5.2.5")
CPMAddPackage("gh:glfw/glfw#3.3.8")
CPMAddPackage("gh:g-truc/glm#0.9.9.8")
CPMAddPackage("gh:ocornut/imgui@1.88")
CPMAddPackage("gh:gabime/spdlog@1.10.0")

add_library(imgui STATIC ${imgui_SOURCE_DIR}/imgui.cpp
					     ${imgui_SOURCE_DIR}/imgui_demo.cpp
					     ${imgui_SOURCE_DIR}/imgui_draw.cpp
						 ${imgui_SOURCE_DIR}/imgui_tables.cpp
					     ${imgui_SOURCE_DIR}/imgui_widgets.cpp)

# ---- Set flags ----

set(GLFW_BUILD_EXAMPLES OFF CACHE INTERNAL "Build the GLFW example programs")
set(GLFW_BUILD_TESTS    OFF CACHE INTERNAL "Build the GLFW test programs")
set(GLFW_BUILD_DOCS     OFF CACHE INTERNAL "Build the GLFW documentation")
set(GLFW_INSTALL        OFF CACHE INTERNAL "Generate installation target")

set(BUILD_SHARED_LIBS         OFF CACHE INTERNAL "Build package with shared libraries.")
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE INTERNAL "If the supplementary tools for Assimp are built in addition to the library.")
set(ASSIMP_BUILD_TESTS        OFF CACHE INTERNAL "If the test suite for Assimp is built in addition to the library.")

# ---- Add subdirectories ----
add_subdirectory(src)