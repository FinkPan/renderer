cmake_minimum_required(VERSION 2.8)

include("cmake/hslib.cmake")
include("cmake/hsprj.cmake")

set(HSPRJ_NAME "hs_renderer")
hs_project(${HSPRJ_NAME})

find_package(OpenGL)
if(NOT OPENGL_FOUND)
  message("ERROR: OpenGL not found")
endif(NOT OPENGL_FOUND)

if(WIN32) #windows 模板需要加 _VARIADIC_MAX=10
  add_definitions(-D_VARIADIC_MAX=10) 
endif() 

###############################################################################
#obj_utility
set(OBJ_UTILITY_NAME "obj_utility")
set(OBJ_UTILITY_SOURCES
  "include/hs_renderer/obj_utility/viewing_transformer.hpp"
  )

hslib_add_library(${OBJ_UTILITY_NAME} ${HSPRJ_NAME}
                  ${OBJ_UTILITY_SOURCES})
#HEADONLY                  
SET_TARGET_PROPERTIES(${OBJ_UTILITY_NAME} PROPERTIES LINKER_LANGUAGE C++)
hslib_add_dep(${OBJ_UTILITY_NAME}
  3RDPARTY_DEPS eigen HEADONLY
  HS_DEPS hs_math HEADONLY
  )

#test_obj_utility
set(OBJ_UTILITY_UTEST_NAME obj_utility_utest)
set(OBJ_UTILITY_UTEST_SOURCES
  "unit_test/test_main.cpp"
  "unit_test/test_obj_utility/test_viewing_transformer.cpp"
  )
  
add_executable(${OBJ_UTILITY_UTEST_NAME} ${OBJ_UTILITY_UTEST_SOURCES})    

hslib_add_dep(${OBJ_UTILITY_UTEST_NAME}
  3RDPARTY_DEPS gtest
  3RDPARTY_DEPS eigen HEADONLY
  3RDPARTY_DEPS freeglut
  3RDPARTY_DEPS glew
  HS_DEPS hs_math HEADONLY
  )
hslib_post_build(${OBJ_UTILITY_UTEST_NAME})

if(OPENGL_FOUND)
  target_link_libraries(${OBJ_UTILITY_UTEST_NAME} ${OPENGL_LIBRARIES})
endif()


###############################################################################
#obj_render
set(OBJ_RENDER_NAME "obj_render")
set(OBJ_RENDER_SOURCES
  "include/hs_renderer/obj_renderer/lod_tiles_loader.hpp"
  "include/hs_renderer/obj_renderer/obj_tiles_loader.hpp"
  "include/hs_renderer/obj_renderer/obj_lod_tiles_loader_engine.hpp"
  "include/hs_renderer/obj_renderer/obj_tile.hpp"
  "include/hs_renderer/obj_renderer/obj_renderer.hpp"
  )

hslib_add_library(${OBJ_RENDER_NAME} ${HSPRJ_NAME}
                  ${OBJ_RENDER_SOURCES})
#HEADONLY                  
SET_TARGET_PROPERTIES(${OBJ_RENDER_NAME} PROPERTIES LINKER_LANGUAGE C++)

#test_obj_tiles_loader
set(OBJ_RENDER_UTEST_NAME obj_render_utest)
set(OBJ_RENDER_UTEST_SOURCES
  "unit_test/test_main.cpp"
  "unit_test/test_obj_renderer/test_obj_tiles_loader.cpp"
  "unit_test/test_obj_renderer/test_obj_lod_tiles_loader_engine.cpp"
  "unit_test/test_obj_renderer/test_obj_tile.cpp"
  "unit_test/test_obj_renderer/test_obj_renderer.cpp"
  )
  
add_executable(${OBJ_RENDER_UTEST_NAME} ${OBJ_RENDER_UTEST_SOURCES})    

hslib_add_dep(${OBJ_RENDER_UTEST_NAME}
  3RDPARTY_DEPS gtest
  )
hslib_post_build(${OBJ_RENDER_UTEST_NAME})