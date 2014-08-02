#include <cstring>
#include <fstream>
#include <iostream>

#include "hs_renderer/renderer/OBJData.hpp"

OBJData::OBJData()
  :has_render_(false), has_load_(false),lod_(0),image_width_(0),image_height_(0)
{
}

OBJData::~OBJData()
{
  std::cout << "ɾ��\n";

//   delete [] vertex_data_;
//   delete [] texturecoord_data_;
//   delete [] index_data_;
//   delete [] image_data_;
}

void OBJData::LoadOBJ(const std::string& full_file_path)
{
  std::cout << "��ȡ " << full_file_path << " �ļ�!\n";
  file_path_ = full_file_path;
  std::ifstream obj_file_path(full_file_path,std::ios_base::binary);
  if (!obj_file_path.is_open())
  {
    std::cout << "�޷���ȡ " << full_file_path << " �ļ�!\n";
    return;
  }
  obj_file_path.read((char*)&lod_,sizeof(int));
  obj_file_path.ignore(sizeof(box_)); //���԰�Χ��

  //��ȡ�����С������
  obj_file_path.read((char*)&vertex_size_,sizeof(size_t));
  vertex_data_ = new float[vertex_size_*sizeof(float)];
  obj_file_path.read((char*)vertex_data_,vertex_size_*sizeof(float));
                                                      
  //��ȡUV��С������
  obj_file_path.read((char*)&texturecoord_size_,sizeof(size_t));
  texturecoord_data_ = new float[texturecoord_size_*sizeof(float)];
  obj_file_path.read((char*)texturecoord_data_,texturecoord_size_*sizeof(float));

  //��ȡindex��С������
  obj_file_path.read((char*)&index_size_,sizeof(size_t));
  index_data_ = new unsigned int[index_size_*sizeof(unsigned int)];
  obj_file_path.read((char*)index_data_,index_size_*sizeof(unsigned int));

  //��ȡimage��С������
  obj_file_path.read((char*)&image_width_,sizeof(int));
  obj_file_path.read((char*)&image_height_,sizeof(int));
  image_data_ = new unsigned char[ image_width_ * image_height_ * 3* sizeof(unsigned char)];
  obj_file_path.read((char*)image_data_,image_width_ * image_height_ * 3 * sizeof(unsigned char));

  obj_file_path.close();
  has_load_ = true;

}


