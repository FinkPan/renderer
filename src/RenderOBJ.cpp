#include "hs_renderer/renderer/RenderOBJ.hpp"
RenderOBJ::RenderOBJ()
{
}

RenderOBJ::~RenderOBJ()
{
}

void RenderOBJ::init()
{
  glGenVertexArrays(1, &vertices_array_object_);
  glGenBuffers(1, &vertices_buffer_object_);
  glGenBuffers(1, &indices_buffer_object_);
}

void RenderOBJ::LoadOBJ_OPENGL(const OBJData& obj_data)
{ 
  glBindBuffer(GL_ARRAY_BUFFER, vertices_buffer_object_);
  glBufferData(GL_ARRAY_BUFFER, obj_data.vertex_size_(),obj_data.vertex_data(), GL_STATIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, texturecoord_buffer_object_);
  glBufferData(GL_ARRAY_BUFFER, obj_data.texturecoord_size(),obj_data.texturecoord_data(), GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_buffer_object_);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, obj_data.index_size(),obj_data.index_data(), GL_STATIC_DRAW);

  glBindVertexArray(vertices_array_object_);

  glBindBuffer(GL_ARRAY_BUFFER, vertices_buffer_object_);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

  glBindBuffer(GL_ARRAY_BUFFER, texturecoord_buffer_object_);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glDisableVertexAttribArray(0);
  glBindVertexArray(0);

}

int RenderOBJ::Render()
{
  glBindVertexArray(vertices_array_object_);
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_buffer_object_);

  glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, 0);

  glDisableVertexAttribArray(0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  return 0;
}

void RenderOBJ::ReleaseGLResource()
{
  glDeleteBuffers(1, &vertices_buffer_object_);
  glDeleteBuffers(1, &indices_buffer_object_);
  glDeleteVertexArrays(1, &vertices_array_object_);
}