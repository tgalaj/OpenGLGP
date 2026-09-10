cmake_minimum_required(VERSION 3.21 FATAL_ERROR)
project(OpenGLGP VERSION 3.0 LANGUAGES C CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(global_settings)
include(CTest)

find_package(OpenGL REQUIRED)

# ---- Dependencies ----
add_subdirectory(thirdparty)

# ---- Application ----
add_subdirectory(src)

if(BUILD_TESTING)
  add_subdirectory(tests)
endif()
