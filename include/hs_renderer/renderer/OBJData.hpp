#ifndef _HS_RENDER_OBJRENDERER_OBJDATA_HPP_
#define _HS_RENDER_OBJRENDERER_OBJDATA_HPP_

#include <string>

class OBJData
{
public:
  OBJData();
  ~OBJData();

  void LoadOBJ(const std::string& full_file_path);
  void UnLoadOBJ();
  inline size_t vertex_size() const { return vertex_size_; }
  inline size_t texturecoord_size() const { return texturecoord_size_; }
  inline size_t index_size() const { return index_size_; }
  inline float* vertex_data() const { return vertex_data_; }
  inline float* texturecoord_data() const { return texturecoord_data_; }
  inline unsigned int* index_data() const { return index_data_; }
  inline int image_width()  const { return image_width_; }
  inline int image_height() const { return image_height_; }
  inline unsigned char* image_data() const { return image_data_; }
  inline bool has_load() const { return has_load_; }
  inline std::string getfilename() const { return file_path_; }

private:
  std::string file_path_;
  bool has_load_;
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
};

#endif