# CMakeList.txt : CMake project for cmakelearnopengl, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (learnopengl "main.cpp" "glad.c"
        Shader.h stb_image.h
        stb_image.cpp "Camera.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET learnopengl PROPERTY CXX_STANDARD 20)
endif()

configure_file(asset_locations.h.in asset_locations.h)
#configure_file(assets/basic.vert assets/basic.vert COPYONLY)
#configure_file(assets/basic.frag assets/basic.frag COPYONLY)

# TODO: Add tests and install targets if needed.
target_include_directories(learnopengl PUBLIC "../include" "${CMAKE_BINARY_DIR}/cmakelearnopengl")
target_link_directories(learnopengl PUBLIC "../lib")
target_link_libraries(learnopengl PUBLIC glfw3 opengl32)
