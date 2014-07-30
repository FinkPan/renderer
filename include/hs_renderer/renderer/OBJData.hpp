#ifndef _HS_RENDER_OBJRENDERER_OBJDATA_HPP_
#define _HS_RENDER_OBJRENDERER_OBJDATA_HPP_

#include <string>

class OBJData
{
public:
  OBJData();
  ~OBJData();

  void LoadOBJ();
  void UnLoadOBJ();

private:
  bool has_ready_;
  int lod_;
  float box_[6]; // minx, miny, minz, maxx, maxy, maxz;
  size_t vertex_size_;
  float *vertex_data_;
  size_t texturecoord_size_;
  float *texturecoord_data_;
  size_t index_size_;
  unsigned int *index_data_;
  int image_width_;
  int image_height_;
  unsigned char* image_data_;
  std::string file_path_;
};

#endif