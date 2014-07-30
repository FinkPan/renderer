#ifndef _HS_RENDER_OBJRENDERER_RENDEROBJ_HPP_
#define _HS_RENDER_OBJRENDERER_RENDEROBJ_HPP_

#include <GL/glew.h>

class RenderOBJ
{
public:
  RenderOBJ();
  ~RenderOBJ();

  void init();

  void LoadOBJ_Opengl();

private:
  GLuint vertices_array_object_;
  GLuint vertices_buffer_object_;
  GLuint indices_buffer_object_;

};

#endif