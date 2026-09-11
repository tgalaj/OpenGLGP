set(OPENGLGP_FRAMEWORK_CORE_SOURCES
  "${CMAKE_CURRENT_SOURCE_DIR}/framework/AssetLocator.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/framework/RunConfig.cpp"
)

add_library(openglgp_framework_core STATIC ${OPENGLGP_FRAMEWORK_CORE_SOURCES})
target_include_directories(openglgp_framework_core PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_compile_features(openglgp_framework_core PUBLIC cxx_std_20)
target_compile_definitions(
  openglgp_framework_core
  PRIVATE OPENGLGP_SOURCE_ASSET_DIR="${CMAKE_SOURCE_DIR}/res"
)
openglgp_enable_warnings(openglgp_framework_core)

# This template intentionally uses CONFIGURE_DEPENDS so students can add files
# under src without editing CMakeLists.txt. CMake will regenerate the build
# system when matching files appear or disappear.
file(GLOB_RECURSE OPENGLGP_SOURCE_FILES CONFIGURE_DEPENDS
  "${CMAKE_CURRENT_SOURCE_DIR}/*.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp"
)

list(REMOVE_ITEM OPENGLGP_SOURCE_FILES ${OPENGLGP_FRAMEWORK_CORE_SOURCES})

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${OPENGLGP_SOURCE_FILES})

add_executable(${PROJECT_NAME} ${OPENGLGP_SOURCE_FILES})

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)
target_compile_definitions(
  ${PROJECT_NAME}
  PRIVATE
    GLFW_INCLUDE_NONE
    $<$<CONFIG:Debug>:OPENGLGP_DEBUG_BUILD=1>
)
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")
target_link_libraries(
  ${PROJECT_NAME}
  PRIVATE
    OpenGL::GL
    glad
    assimp
    glfw
    glm::glm
    imgui
    openglgp_framework_core
    spdlog::spdlog
    stb_image
)
openglgp_enable_warnings(${PROJECT_NAME})

if(MSVC)
  target_compile_definitions(${PROJECT_NAME} PRIVATE NOMINMAX WIN32_LEAN_AND_MEAN)
endif()

set_target_properties(
  ${PROJECT_NAME}
  openglgp_framework_core
  PROPERTIES FOLDER "OpenGLGP"
)

if(EXISTS "${CMAKE_SOURCE_DIR}/res")
  add_custom_command(
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND
      ${CMAKE_COMMAND} -E copy_directory
      "${CMAKE_SOURCE_DIR}/res"
      "$<TARGET_FILE_DIR:${PROJECT_NAME}>/res"
    COMMENT "Copying runtime resources"
    VERBATIM
  )
endif()
