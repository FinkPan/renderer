#ifndef HS_RENDERER_OBJ_LOD_TILES_LOADER_ENGINE_HPP
#define HS_RENDERER_OBJ_LOD_TILES_LOADER_ENGINE_HPP

#include <string>
#include <map>
#include <sstream>
#include <fstream>
#include <iostream>

#include "hs_renderer/obj_renderer/obj_tile.hpp"

namespace hs
{
namespace render
{
namespace obj_render
{

template <typename _Scalar>
class OBJLODTilesLoaderEngine
{
public:
  typedef _Scalar Scalar;
  typedef OBJTile<Scalar> OBJTile;
  typedef unsigned int IndexType;
  typedef IndexType* PIndexType;
  typedef unsigned char ImageType;
  typedef ImageType* PImageType;


OBJLODTilesLoaderEngine()
{
  obj_tile_ = new OBJTile;
}
OBJLODTilesLoaderEngine(const std::string& obj_path)
{
  obj_tile_ = new OBJTile;
  obj_path_ = obj_path;
  Open();
}

~OBJLODTilesLoaderEngine()
{
  delete obj_tile_;
}

const OBJTile* GetOBJData() const { return obj_tile_;}

private:
int Open()
{
  //std::cout << "读取 " << obj_path_ << " 文件!\n";
  std::ifstream in_obj_file(obj_path_,std::ios_base::binary);
  if (!in_obj_file.is_open())
  {
    std::cout << "无法读取 " << obj_path_ << " 文件!\n";
    return -1;
  }

  in_obj_file.read((char*)&obj_tile_->SetLod(),sizeof(int));
  in_obj_file.read((char*)obj_tile_->BoundingBox(),6*sizeof(Scalar));
 
  //读取顶点大小及数据
  in_obj_file.read((char*)&obj_tile_->VertexSize(),sizeof(size_t));
  in_obj_file.read((char*)obj_tile_->VertexData(),
                      obj_tile_->VertexSize()*sizeof(Scalar));

  //读取UV大小及数据
  in_obj_file.read((char*)&obj_tile_->UVSize(),sizeof(size_t));
  in_obj_file.read((char*)obj_tile_->UVData(),
                     obj_tile_->UVSize()*sizeof(Scalar));

  //读取index大小及数据
  in_obj_file.read((char*)&obj_tile_->IndexSize(),sizeof(size_t));
  in_obj_file.read((char*)obj_tile_->IndexData(),
                      obj_tile_->IndexSize()*sizeof(IndexType));

  //读取image大小及数据
  in_obj_file.read((char*)&obj_tile_->ImageWidth(),sizeof(int));
  in_obj_file.read((char*)&obj_tile_->ImageHeight(),sizeof(int));
  in_obj_file.read((char*)obj_tile_->ImageData(),
                      obj_tile_->ImageWidth() * obj_tile_->ImageHeight() * 3 *
                      sizeof(unsigned char));

  in_obj_file.close();

  return 0;
}

private:
  OBJTile* obj_tile_;
  std::string obj_path_;
};



}//namespace obj_render
}//namespace render
}//namespace hs

#endif