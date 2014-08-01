#ifndef _HS_RENDER_OBJRENDERER_RENDEROBJ_HPP_
#define _HS_RENDER_OBJRENDERER_RENDEROBJ_HPP_

#include <vector>

#include <GL/glew.h>
#include "hs_renderer/renderer/OBJData.hpp"

class RenderOBJ
{
public:
  RenderOBJ();
  ~RenderOBJ();

  void init();
  void LoadOBJ_OPENGL(const OBJData& obj_data);
  int Render();
  void ReleaseGLResource();

private:
  std::vector<OBJData> vecOBJData_;

  GLuint vertices_array_object_;
  GLuint vertices_buffer_object_;
  GLuint texturecoord_buffer_object_;
  GLuint indices_buffer_object_;

};

#endif