cmake_minimum_required(VERSION 2.8)

include("cmake/hslib.cmake")
include("cmake/hsprj.cmake")

set(HSPRJ_NAME "hs_renderer")
hs_project(${HSPRJ_NAME})

find_package(OpenGL)
if(NOT OPENGL_FOUND)
  message("ERROR: OpenGL not found")
endif(NOT OPENGL_FOUND)


#obj_renderer
set(RENDER_NAME "obj_renderer")
set(RENDER_SOURCES
  "include/hs_renderer/obj_renderer/obj_renderer.hpp"
  )
 if(WIN32) #windows 模板需要加 _VARIADIC_MAX=10
 add_definitions(-D_VARIADIC_MAX=10) 
endif() 

hslib_add_library(${RENDER_NAME} ${HSPRJ_NAME}
                  ${RENDER_SOURCES})
SET_TARGET_PROPERTIES(${RENDER_NAME} PROPERTIES LINKER_LANGUAGE C++)
                  
#test_obj_renderer
set(RENDER_UTEST_SOURCES
  "unit_test/test_main.cpp"
  "unit_test/test_obj_renderer/test_obj_renderer.cpp"
  )
hslib_add_utest(${RENDER_NAME} SOURCES
  ${RENDER_UTEST_SOURCES})
hslib_add_dep(${RENDER_NAME}_utest
  )
hslib_post_build(${RENDER_NAME}_utest)
                                  
if(OPENGL_FOUND)
  target_link_libraries(${RENDER_NAME}_utest ${OPENGL_LIBRARIES})
endif()

###############################################################################
#obj_lod_tiles_loader_engine
set(OBJ_LOD_TILES_LOADER_NAME "obj_lod_tiles_loader_engine")
set(OBJ_LOD_TILES_LOADER_SOURCES
  "include/hs_renderer/obj_renderer/obj_lod_tiles_loader_engine.hpp"
  )
#if(WIN32) #windows 模板需要加 _VARIADIC_MAX=10
# add_definitions(-D_VARIADIC_MAX=10) 
#endif() 

hslib_add_library(${OBJ_LOD_TILES_LOADER_NAME} ${HSPRJ_NAME}
                  ${OBJ_LOD_TILES_LOADER_SOURCES})
#HEADONLY                  
SET_TARGET_PROPERTIES(${OBJ_LOD_TILES_LOADER_NAME} PROPERTIES LINKER_LANGUAGE C++)
                  
#test_lod_tiles_loader_engine
set(OBJ_LOD_TILES_LOADER_UTEST_NAME obj_lod_tiles_loader_engine_utest)
set(OBJ_LOD_TILES_LOADER_UTEST_SOURCES
  "unit_test/test_main.cpp"
  "unit_test/test_obj_renderer/test_obj_lod_tiles_loader_engine.cpp"
  )
  
add_executable(${OBJ_LOD_TILES_LOADER_UTEST_NAME} ${OBJ_LOD_TILES_LOADER_UTEST_SOURCES})    

hslib_add_dep(${OBJ_LOD_TILES_LOADER_NAME}_utest
  3RDPARTY_DEPS gtest
  )
hslib_post_build(${OBJ_LOD_TILES_LOADER_NAME}_utest)
                                 

###############################################################################
#obj_tile
set(OBJ_TILE_NAME "obj_tile")
set(OBJ_TILE_SOURCES
  "include/hs_renderer/obj_renderer/obj_tile.hpp"
  )
#if(WIN32) #windows 模板需要加 _VARIADIC_MAX=10
# add_definitions(-D_VARIADIC_MAX=10) 
#endif() 

hslib_add_library(${OBJ_TILE_NAME} ${HSPRJ_NAME}
                  ${OBJ_TILE_SOURCES})
#HEADONLY                   
SET_TARGET_PROPERTIES(${OBJ_TILE_NAME} PROPERTIES LINKER_LANGUAGE C++)
              
#test_obj_tile
set(OBJ_TILE_UTEST_NAME obj_tile_utest)
set(OBJ_TILE_UTEST_SOURCES
  "unit_test/test_main.cpp"
  "unit_test/test_obj_renderer/test_obj_tile.cpp"
  )
  
add_executable(${OBJ_TILE_UTEST_NAME} ${OBJ_TILE_UTEST_SOURCES})  
#hslib_add_utest(${OBJ_TILE_NAME} LOCAL_DEPS ${OBJ_TILE_NAME} SOURCES
#  ${OBJ_TILE_UTEST_SOURCES})
hslib_add_dep(${OBJ_TILE_UTEST_NAME}
  3RDPARTY_DEPS gtest
  )
hslib_post_build(${OBJ_TILE_UTEST_NAME})
