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
namespace renderer
{
namespace render
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
  typedef int LODid;

  struct LODBaseInfo
  {
    Scalar bounding_box[6];
    std::string obj_path;
  };
  typedef std::multimap<LODid, LODBaseInfo> LODBaseInfoContainer;


OBJLODTilesLoaderEngine()
{
  obj_tile_ = new OBJTile;
}
OBJLODTilesLoaderEngine(const std::string& obj_list_path)
{
  obj_tile_ = new OBJTile;
  obj_list_path_ = obj_list_path;
  Open();
}

~OBJLODTilesLoaderEngine()
{
  delete obj_tile_;
}

const OBJTile* GetOBJData() const { return obj_tile_;}

void LoadTile(size_t lod_id, Scalar* tile_id, OBJTile& tile)
{
  //����lod
  auto itr_lod = lod_base_infos_.find(lod_id);
  if (itr_lod != lod_base_infos_.end())
  {

  }
}

bool IsValid() const
{
  return obj_tile_ != nullptr; && !lod_base_infos_.empty();
}
private:
//��ȡlist.txt�ļ�
//14 21
//E:\workspace\obj\tianyang_gonglian_bin\Tile_+005_+005
//...
int Open()
{
  int lod_min = 0, lod_max = 0;
  std::ifstream list_file(obj_list_path_,std::ifstream::in);
  if (!list_file.is_open())
  {
    std::cout << "�޷���ȡ " << obj_list_path_ << " �ļ�!\n";
    return -1;
  }
  std::string file_name;
  if (list_file)
  {
    list_file  >> lod_min  >> lod_max;
    while(!list_file.eof())
    {
      list_file >> file_name;
      if (file_name.empty())  //���к���
        break;
      AnalysisFilePath(file_name, lod_min, lod_max); 
      file_name.clear();  //����
    }
  }
  list_file.close();

  return 0;
}

void AnalysisFilePath(std::string &file_name, int &lod_min, int &lod_max)
{
  std::string file_name_id;
  LODBaseInfo file_head;
  size_t itr_slash_pos = file_name.rfind("\\");  //�ֽ���ļ���  
  if (itr_slash_pos != std::string::npos)
    file_name_id = file_name.substr(itr_slash_pos+1, file_name.size());
  std::stringstream ss;
  std::string str_lod;
  for (int i = lod_min; i <= lod_max; ++i)
  {
    ss.clear();
    ss <<  i;
    ss >> str_lod;
    file_head.obj_path  = file_name + "\\" + file_name_id + "_L" + str_lod + ".bin";
    ReadOBJBoundingBox(file_head.obj_path, file_head);
    lod_base_infos_.insert(std::make_pair(i,file_head));
  }
}

void ReadOBJBoundingBox(const std::string &obj_path, LODBaseInfo& lod_base_info)
{
  //std::cout << "��ȡ " << obj_path_ << " �ļ�!\n";
  std::ifstream in_obj_file(obj_path,std::ios_base::binary);
  if (!in_obj_file.is_open())
  {
    std::cout << "�޷���ȡ " << obj_path << " �ļ�!\n";
    return;
  }
  in_obj_file.ignore(sizeof(int));
  //in_obj_file.read((char*)&lod_base_info.lod_index,sizeof(int));
  in_obj_file.read((char*)lod_base_info.bounding_box,6*sizeof(Scalar));

  in_obj_file.close();

}

//��ȡobj�ļ�
int ReadOBJFile(std::string obj_path)
{
  //std::cout << "��ȡ " << obj_path_ << " �ļ�!\n";
  std::ifstream in_obj_file(obj_path,std::ios_base::binary);
  if (!in_obj_file.is_open())
  {
    std::cout << "�޷���ȡ " << obj_path << " �ļ�!\n";
    return -1;
  }

  in_obj_file.read((char*)&obj_tile_->SetLod(),sizeof(int));
  in_obj_file.read((char*)obj_tile_->BoundingBox(),6*sizeof(Scalar));

  //��ȡ�����С������
  in_obj_file.read((char*)&obj_tile_->VertexSize(),sizeof(size_t));
  in_obj_file.read((char*)obj_tile_->VertexData(),
    obj_tile_->VertexSize()*sizeof(Scalar));

  //��ȡUV��С������
  in_obj_file.read((char*)&obj_tile_->UVSize(),sizeof(size_t));
  in_obj_file.read((char*)obj_tile_->UVData(),
    obj_tile_->UVSize()*sizeof(Scalar));

  //��ȡindex��С������
  in_obj_file.read((char*)&obj_tile_->IndexSize(),sizeof(size_t));
  in_obj_file.read((char*)obj_tile_->IndexData(),
    obj_tile_->IndexSize()*sizeof(IndexType));

  //��ȡimage��С������
  in_obj_file.read((char*)&obj_tile_->ImageWidth(),sizeof(int));
  in_obj_file.read((char*)&obj_tile_->ImageHeight(),sizeof(int));
  in_obj_file.read((char*)obj_tile_->ImageData(),
    obj_tile_->ImageWidth() * obj_tile_->ImageHeight() * 3 *
    sizeof(unsigned char));

  in_obj_file.close();

  return 0;

}

//���ݰ�Χ�в��Ҷ�Ӧ�Ŀ�

private:
  OBJTile* obj_tile_;
  std::string obj_list_path_;
  LODBaseInfoContainer  lod_base_infos_;
};



}//namespace obj_render
}//namespace render
}//namespace hs

#endif