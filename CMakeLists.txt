cmake_minimum_required(VERSION 2.8)

include("cmake/hslib.cmake")
include("cmake/hsprj.cmake")

set(HSPRJ_NAME "hs_render")
hs_project(${HSPRJ_NAME})

find_package(OpenGL)
if(NOT OPENGL_FOUND)
  message("ERROR: OpenGL not found")
endif(NOT OPENGL_FOUND)


#OBJRenderer
set(RENDER_NAME "OBJRenderer")
set(RENDER_SOURCES
    "include/hs_renderer/renderer/OBJLoader.hpp"
    "include/hs_renderer/renderer/OBJData.hpp"
    "include/hs_renderer/renderer/OBJRenderer.hpp"
    "include/hs_renderer/renderer/RenderOBJ.hpp"
    "include/hs_renderer/renderer/ViewTransformer.hpp"
    "src/OBJLoader.cpp"
    "src/OBJData.cpp"
    "src/OBJRenderer.cpp"
    "src/RenderOBJ.cpp"
    "src/ViewTransformer.cpp"
    "src/main.cpp"
  )
 if(WIN32) #windows 模板需要加 _VARIADIC_MAX=10
 add_definitions(-D_VARIADIC_MAX=10) 
endif() 

include_directories(include)
add_executable(${RENDER_NAME} ${RENDER_SOURCES})

hslib_add_dep(${RENDER_NAME}
  3RDPARTY_DEPS glew DEP_LIBS glew
  3RDPARTY_DEPS freeglut
  3RDPARTY_DEPS glm HEADONLY
  )
hslib_post_build(${RENDER_NAME})
if(OPENGL_FOUND)
  target_link_libraries(${RENDER_NAME} ${OPENGL_LIBRARIES})
endif()                  
