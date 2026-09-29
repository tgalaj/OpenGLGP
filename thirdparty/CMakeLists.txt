include(CPM)

set(GLAD_DIR "${CMAKE_CURRENT_LIST_DIR}/glad")
add_library(glad STATIC "${GLAD_DIR}/src/glad.c")
target_include_directories(glad PUBLIC "${GLAD_DIR}/include")
target_compile_features(glad PRIVATE c_std_11)

CPMAddPackage(
  NAME glfw
  GITHUB_REPOSITORY glfw/glfw
  GIT_TAG 3.4
)
CPMAddPackage(
  NAME glm
  GITHUB_REPOSITORY g-truc/glm
  GIT_TAG 1.0.1
)
CPMAddPackage(
  NAME assimp
  GITHUB_REPOSITORY assimp/assimp
  GIT_TAG v5.4.3
)
CPMAddPackage(
  NAME spdlog
  GITHUB_REPOSITORY gabime/spdlog
  GIT_TAG v1.14.1
)
CPMAddPackage(
  NAME imgui_source
  GITHUB_REPOSITORY ocornut/imgui
  GIT_TAG v1.91.9b
  DOWNLOAD_ONLY YES
)
CPMAddPackage(
  NAME stb_source
  GITHUB_REPOSITORY nothings/stb
  GIT_TAG f0569113c93ad095470c54bf34a17b36646bbbb5
  DOWNLOAD_ONLY YES
)

add_library(imgui STATIC
  "${imgui_source_SOURCE_DIR}/imgui.cpp"
  "${imgui_source_SOURCE_DIR}/imgui_draw.cpp"
  "${imgui_source_SOURCE_DIR}/imgui_tables.cpp"
  "${imgui_source_SOURCE_DIR}/imgui_widgets.cpp"
  "${imgui_source_SOURCE_DIR}/backends/imgui_impl_glfw.cpp"
  "${imgui_source_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp"
)
target_include_directories(imgui
  PUBLIC
    "${imgui_source_SOURCE_DIR}"
    "${imgui_source_SOURCE_DIR}/backends"
)
target_link_libraries(imgui PUBLIC glfw)

add_library(stb_image STATIC stb_image.cpp)
target_include_directories(stb_image PUBLIC "${stb_source_SOURCE_DIR}")

set_target_properties(
  glad
  assimp
  glfw
  glm
  imgui
  spdlog
  stb_image
  PROPERTIES FOLDER "thirdparty"
)
