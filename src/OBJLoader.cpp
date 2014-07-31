#include "hs_renderer/renderer/OBJLoader.hpp"
#include <fstream>
#include <iostream>
#include <sstream> 

#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>


OBJLoader::OBJLoader() :
  lod_min_(0),lod_max_(0),box_finish_(false)
{
  box_[0] = 0.0f; box_[1] = 0.0f; box_[2] = 0.0f;
  box_[3] = 0.0f; box_[4] = 0.0f; box_[5] = 0.0f;
}

OBJLoader::~OBJLoader()
{
  if (calc_box_thread_.joinable())
  {
    calc_box_thread_.join();
  }
}

void OBJLoader::Start()
{
  keep_working_ = 1;
  working_thread_ = std::thread(&OBJLoader::Work, this);
}

void OBJLoader::Stop()
{
  keep_loading_ = 0;
  keep_working_ = 0;
  if (working_thread_.joinable())
  {
    working_thread_.join();
  }
}

void OBJLoader::Load(const std::string &boj_path)
{
  std::cout << "Load = " << boj_path  <<  "\n";

}

void OBJLoader::Work()
{
  std::thread::id main_thread_id = std::this_thread::get_id();
  std::cout << "Work = " << main_thread_id << "\n";
  std::string current_obj_path;

  while (keep_working_)
  {
    //默认加载最高层第一个obj    
    Lock();
     auto itr_key = m_FileInfo_.find((int)current_lod_);
     if (itr_key != m_FileInfo_.end())
     {
      // Load(current_obj_path);
     }
    Unlock();
  }

}

//文件格式:
//14 20
//E:\workspace\obj\tianyang_gonglian\Tile_+003_+004
//...
void OBJLoader::LoadOBJ(std::string file_path)
{
  std::string file_name;
  int lod_min = 0, lod_max = 0;
  std::ifstream list_file(file_path);
  if (list_file)
  {
    list_file  >> lod_min  >> lod_max;
    lod_min_ = lod_min; lod_max_ = lod_max;
    while(!list_file.eof())
    {
      list_file >> file_name;
      AnalysisFilePath(file_name, lod_min, lod_max); 
    }
  }
  list_file.close();
  calc_box_thread_ = std::thread(&OBJLoader::CalculateBox, this);
  std::cout << "LoadOBJ!!!!!!!!!!\n";

}

void OBJLoader::InitializeViewTransformer(ViewTransformer& view_transformer)
{
  while(1)
  {
    if (box_finish_)
    {
      view_transformer.SetProjection(box_[0],box_[3],box_[1],box_[4]);
      break;
    }
  }
}

void OBJLoader::UpdateLoadingOBJ(const ViewTransformer& vt)
{
  std::string str_load;
  float left = vt.left();
  float right= vt.right();
  float bottom=vt.bottom();
  float top   =vt.top();
  glm::mat4 model = vt.GetModel();
  glm::vec4 box0,box1, box2, box3, box4, box5, box6, box7;

  Lock();

  current_lod_ = vt.lod();
  auto itr_obj_path = m_FileInfo_.find(current_lod_);
  size_t itr_obj_path_count = m_FileInfo_.count(current_lod_);
  for (int i = 0; i != itr_obj_path_count; ++i,++itr_obj_path)
  {
    box0 = glm::vec4(itr_obj_path->second.box[0],itr_obj_path->second.box[1], itr_obj_path->second.box[5],  1.0f);
    box1 = glm::vec4(itr_obj_path->second.box[0],itr_obj_path->second.box[4], itr_obj_path->second.box[5],  1.0f);
    box2 = glm::vec4(itr_obj_path->second.box[3],itr_obj_path->second.box[4], itr_obj_path->second.box[5],  1.0f);
    box3 = glm::vec4(itr_obj_path->second.box[3],itr_obj_path->second.box[1], itr_obj_path->second.box[5],  1.0f);
    box4 = glm::vec4(itr_obj_path->second.box[0],itr_obj_path->second.box[1], itr_obj_path->second.box[2],  1.0f);
    box5 = glm::vec4(itr_obj_path->second.box[0],itr_obj_path->second.box[4], itr_obj_path->second.box[2],  1.0f);
    box6 = glm::vec4(itr_obj_path->second.box[3],itr_obj_path->second.box[4], itr_obj_path->second.box[2],  1.0f);
    box7 = glm::vec4(itr_obj_path->second.box[3],itr_obj_path->second.box[1], itr_obj_path->second.box[2],  1.0f);


     if(IsNeededLoad(itr_obj_path->second.box))
       needed_load_.push_back(itr_obj_path->second.file_path);
  }

  Unlock();

}

void OBJLoader::GetLoadedMaterial(std::vector<std::string>& vec_str)
{
  std::string str_file;
  Lock();

  Unlock();
}

void OBJLoader::AnalysisFilePath(std::string &file_name, int &lod_min, int &lod_max)
{
   std::string file_name_id;
   FileHead file_head;
   size_t itr_slash_pos = file_name.rfind("\\");  //分解出文件名  
   if (itr_slash_pos != std::string::npos)
     file_name_id = file_name.substr(itr_slash_pos+1, file_name.size());
   std::stringstream ss;
   std::string str_lod;
   for (int i = lod_min; i <= lod_max; ++i)
   {
     ss.clear();
     ss <<  i;
     ss >> str_lod;
     file_head.file_path  = file_name + "\\" + file_name_id + "_" + str_lod + ".bin";
     m_FileInfo_.insert(std::make_pair(i,file_head));
   }
}

void OBJLoader::CalculateBox()
{
  std::thread::id main_thread_id = std::this_thread::get_id();
  std::cout << "CalculateBox = " << main_thread_id << "\n";
  std::ifstream obj_file_path;
  Lock();
  auto itr_obj_path = m_FileInfo_.begin();
  auto itr_obj_path_end = m_FileInfo_.end();
  int lod; 
  for (; itr_obj_path != itr_obj_path_end; ++ itr_obj_path)
  {
    obj_file_path.open(itr_obj_path->second.file_path,std::ios_base::binary | std::ios_base::in);
    if (obj_file_path.is_open())
    {
      obj_file_path.read((char*)&lod,sizeof(int));
      obj_file_path.read((char*)&(itr_obj_path->second.box),sizeof(itr_obj_path->second.box));
      obj_file_path.close();
      itr_obj_path->second.data_finsh = true;
      box_[0] = box_[0] < itr_obj_path->second.box[0] ? box_[0] : itr_obj_path->second.box[0];
      box_[1] = box_[1] < itr_obj_path->second.box[1] ? box_[1] : itr_obj_path->second.box[1];
      box_[2] = box_[2] < itr_obj_path->second.box[2] ? box_[2] : itr_obj_path->second.box[2];
      box_[3] = box_[3] > itr_obj_path->second.box[3] ? box_[3] : itr_obj_path->second.box[3];
      box_[4] = box_[4] > itr_obj_path->second.box[4] ? box_[4] : itr_obj_path->second.box[4];
      box_[5] = box_[5] > itr_obj_path->second.box[5] ? box_[5] : itr_obj_path->second.box[5];
    }
  }
  box_finish_ = true;
  Unlock();
}

bool OBJLoader::IsNeededLoad(float* box)
{

  return true;
}
