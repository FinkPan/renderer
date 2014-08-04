
#include <cstring>
#include <fstream>
#include <iostream>

#include "hs_renderer/renderer/OBJData.hpp"

OBJData::OBJData()
  :lod_(0),image_width_(0),image_height_(0),has_render_(false)
{
}

OBJData::~OBJData()
{
  //std::cout << "删除OBJData.\n";
  //delete [] vertex_data_;
  //delete [] texturecoord_data_;
  //delete [] index_data_;
  //delete [] image_data_;

}

void OBJData::LoadOBJ(const std::string& full_file_path)
{
  std::cout << "读取 " << full_file_path << " 文件!\n";
  file_path_ = full_file_path;
  std::ifstream obj_file_path(full_file_path,std::ios_base::binary);
  if (!obj_file_path.is_open())
  {
    std::cout << "无法读取 " << full_file_path << " 文件!\n";
    return;
  }
  obj_file_path.read((char*)&lod_,sizeof(int));
  //obj_file_path.ignore(sizeof(box_)); //忽略包围盒
  obj_file_path.read((char*)box_,6*sizeof(float));

  //读取顶点大小及数据
  obj_file_path.read((char*)&vertex_size_,sizeof(size_t));
  vertex_data_ = new float[vertex_size_];
  obj_file_path.read((char*)vertex_data_,vertex_size_*sizeof(float));
                                                      
  //读取UV大小及数据
  obj_file_path.read((char*)&texturecoord_size_,sizeof(size_t));
  texturecoord_data_ = new float[texturecoord_size_];
  obj_file_path.read((char*)texturecoord_data_,texturecoord_size_*sizeof(float));

  //读取index大小及数据
  obj_file_path.read((char*)&index_size_,sizeof(size_t));
  index_data_ = new unsigned int[index_size_];
  obj_file_path.read((char*)index_data_,index_size_*sizeof(unsigned int));

  //读取image大小及数据
  obj_file_path.read((char*)&image_width_,sizeof(int));
  obj_file_path.read((char*)&image_height_,sizeof(int));
  image_data_ = new unsigned char[ image_width_ * image_height_ * 3];
  obj_file_path.read((char*)image_data_,image_width_ * image_height_ * 3 * sizeof(unsigned char));

  obj_file_path.close();

}


