cmake_minimum_required(VERSION 2.8)

include("cmake/hslib.cmake")
include("cmake/hsprj.cmake")

set(HSPRJ_NAME "hs_renderer")
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

#test_OBJData
set(OBJDATA_UTEST_NAME "utest_OBJData")
set(OBJDATA_UTEST_SOURCES
  "src/OBJData.cpp"
  "include/hs_renderer/renderer/OBJData.hpp"
  "unit_test/test_main.cpp"
  "unit_test/test_OBJData/test_OBJData.cpp")
add_executable(${OBJDATA_UTEST_NAME} ${OBJDATA_UTEST_SOURCES})
hslib_add_dep(${OBJDATA_UTEST_NAME}
  3RDPARTY_DEPS gtest
  3RDPARTY_DEPS tiff DEP_LIBS tiff
  3RDPARTY_DEPS jpeg DEP_LIBS jpeg
  3RDPARTY_DEPS png DEP_LIBS png
  3RDPARTY_DEPS z DEP_LIBS z
  HS_DEPS hs_image_io DEP_LIBS whole_io
  )
hslib_post_build(${OBJDATA_UTEST_NAME}) 
               
#test_RenderOBJ
set(RENDEROBJ_UTEST_NAME "utest_RenderOBJ")
set(RENDEROBJ_UTEST_SOURCES
  "src/OBJData.cpp"
  "src/RenderOBJ.cpp"
  "include/hs_renderer/renderer/OBJData.hpp"
  "include/hs_renderer/renderer/RenderOBJ.hpp"
  "unit_test/test_main.cpp"
  "unit_test/test_RenderOBJ/test_RenderOBJ.cpp")
add_executable(${RENDEROBJ_UTEST_NAME} ${RENDEROBJ_UTEST_SOURCES})
hslib_add_dep(${RENDEROBJ_UTEST_NAME}
  3RDPARTY_DEPS gtest
  3RDPARTY_DEPS glew DEP_LIBS glew
  3RDPARTY_DEPS glfw
  3RDPARTY_DEPS glm HEADONLY
  )
hslib_post_build(${RENDEROBJ_UTEST_NAME}) 
if(OPENGL_FOUND)
  target_link_libraries(${RENDEROBJ_UTEST_NAME} ${OPENGL_LIBRARIES})
endif() 


#test_OBJLoader
set(OBJLOADER_UTEST_NAME "utest_OBJLoader")
set(OBJLOADER_UTEST_SOURCES
  "src/OBJLoader.cpp"
  "include/hs_renderer/renderer/OBJLoader.hpp"
  "unit_test/test_main.cpp"
  "unit_test/test_OBJLoader/test_OBJLoader.cpp")
add_executable(${OBJLOADER_UTEST_NAME} ${OBJLOADER_UTEST_SOURCES})
hslib_add_dep(${OBJLOADER_UTEST_NAME}
  3RDPARTY_DEPS gtest
  3RDPARTY_DEPS glm HEADONLY
  )
hslib_post_build(${OBJLOADER_UTEST_NAME}) 