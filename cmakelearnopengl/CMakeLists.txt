# CMakeList.txt : CMake project for cmakelearnopengl, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (learnopengl "main.cpp" "glad.c"
        Shader.h stb_image.h
        stb_image.cpp "Camera.h" "VertexArrayObject.h" "VertexArrayObject.cpp"
        Cube.cpp
        Cube.hpp)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET learnopengl PROPERTY CXX_STANDARD 20)
endif()

configure_file(asset_locations.h.in asset_locations.h)
#configure_file(assets/basic.vert assets/basic.vert COPYONLY)
#configure_file(assets/basic.frag assets/basic.frag COPYONLY)

include(FetchContent)
FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG 3.3.8
)
FetchContent_MakeAvailable(glfw)

# TODO: Add tests and install targets if needed.
target_include_directories(learnopengl PUBLIC "../include" "${CMAKE_BINARY_DIR}/cmakelearnopengl")
#target_link_directories(learnopengl PUBLIC "../lib")

target_link_libraries(learnopengl PUBLIC glfw)
if (${UNIX})
  target_link_libraries(learnopengl PUBLIC GL wayland-client)
endif ()
if (${WIN32})
  target_link_libraries(learnopengl PUBLIC opengl32)
endif ()
