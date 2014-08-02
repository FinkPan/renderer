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
  glGenBuffers(1,&texturecoord_buffer_object_);
  glGenBuffers(1, &indices_buffer_object_);
  glGenTextures(1, &texture_buffer_object_);
}

void RenderOBJ::LoadOBJ_OPENGL(OBJData& obj_data)
{ 
  glBindBuffer(GL_ARRAY_BUFFER, vertices_buffer_object_);
  glBufferData(GL_ARRAY_BUFFER, obj_data.vertex_size()*sizeof(float),obj_data.vertex_data(), GL_STATIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, texturecoord_buffer_object_);
  glBufferData(GL_ARRAY_BUFFER, obj_data.texturecoord_size()*sizeof(float),obj_data.texturecoord_data(), GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_buffer_object_);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, obj_data.index_size()*sizeof(unsigned int),obj_data.index_data(), GL_STATIC_DRAW);

  glBindTexture(GL_TEXTURE_2D, texture_buffer_object_);
  glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,obj_data.image_width(),obj_data.image_height(),
    0,GL_RGB,GL_UNSIGNED_BYTE,obj_data.image_data());
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0); //最大尺寸的level限制
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 3);  //最小尺寸的mipmap level限制
  glGenerateMipmap(GL_TEXTURE_2D);  //创建Mipmaps
  glBindTexture(GL_TEXTURE_2D,0);


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

  obj_data.set_has_load(true);
  index_size_ = obj_data.index_size();
}

int RenderOBJ::Render()
{
  glBindVertexArray(vertices_array_object_);
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_buffer_object_);
  glBindTexture(GL_TEXTURE_2D, texture_buffer_object_);

  glDrawElements(GL_TRIANGLES, index_size_, GL_UNSIGNED_INT, 0);

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  return 0;
}

void RenderOBJ::ReleaseGLResource()
{
  glDeleteBuffers(1, &vertices_buffer_object_);
  glDeleteBuffers(1, &indices_buffer_object_);
  glDeleteBuffers(1, &texturecoord_buffer_object_);
  glDeleteTextures(1, &texture_buffer_object_);
  glDeleteVertexArrays(1, &vertices_array_object_);
}
