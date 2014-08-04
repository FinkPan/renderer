#ifndef _HS_RENDER_OBJRENDERER_OBJRENDERER_HPP_
#define _HS_RENDER_OBJRENDERER_OBJRENDERER_HPP_

#include <vector>
#include <iostream>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr
#include <glm/gtc/matrix_transform.hpp>

#include "hs_renderer/renderer/RenderOBJ.hpp"
#include "hs_renderer/renderer/ViewTransformer.hpp"

class OBJRenderer
{
public:
  OBJRenderer();
  ~OBJRenderer();

  void init();
  void LoadData(const RenderOBJ& robj);
  void Render(ViewTransformer& vt);
  void UnLoadData();

private:
  std::vector<RenderOBJ> vec_render_obj_;
  GLuint program_id_;
  GLuint location_mvp_;
  GLuint location_color_;

  int LoadShader(const char* vs, const char* fs);
  static const char* vertex_shader_source;
  static const char* fragment_shader_source;

};

#endif