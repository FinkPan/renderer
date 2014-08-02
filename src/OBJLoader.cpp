#include "hs_renderer/renderer/OBJLoader.hpp"
#include <fstream>
#include <iostream>
#include <sstream> 
#include <algorithm>

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
  OBJData obj_data;

  while (keep_working_)
  {
    Lock();
    //如果lod改变,则清空vecOBJData_;
    if (!vecOBJData_.empty())
    {
      if (vecOBJData_[0].lod() != current_lod_)
      {
        auto itr_obj_path1 = m_FileInfo_.find(vecOBJData_[0].lod());
        size_t itr_obj_path_count1 = m_FileInfo_.count(current_lod_);
        for (int i = 0; i != itr_obj_path_count1; ++i,++itr_obj_path1)
        {
          itr_obj_path1->second.has_load = false;
        }
        vecOBJData_.clear();

      }
    }

    //默认加载最高层第一个obj    
    auto itr_obj_path = m_FileInfo_.find(current_lod_);
    size_t itr_obj_path_count = m_FileInfo_.count(current_lod_);
    for (int i = 0; i != itr_obj_path_count; ++i,++itr_obj_path)
    {

      if (itr_obj_path->second.needed_load == true) //该obj需要加载
      {

        if (itr_obj_path->second.has_load == false) //该obj没有加载
        {
          obj_data.LoadOBJ(itr_obj_path->second.file_path);
          itr_obj_path->second.has_load = true;
          vecOBJData_.push_back(obj_data);
        }
      }
      else   //不需要加载,如果已经加载则删掉该obj
      {  
        for (auto itr_pos = vecOBJData_.begin(); itr_pos != vecOBJData_.end(); ++itr_pos)
        {
          if (itr_pos->getfilename() == itr_obj_path->second.file_path)
          {
            std::cout << "删除" << itr_obj_path->second.file_path << "\n";
            vecOBJData_.erase(itr_pos);
          }                         
        }
      }
    }
    Unlock();
  }//  while (keep_working_)
}

//文件格式:
//14 20
//E:\workspace\obj\tianyang_gonglian\Tile_+003_+004
//...
void OBJLoader::LoadOBJ(const std::string& file_path)
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
  //std::vector<std::string>::iterator itr_pos = needed_load_.begin();

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
    //与变化过的model矩阵相乘;
    box0 = model * box0; box1 = model * box1; box2 = model * box2; box3 = model * box3; box4 = model * box4;
    box5 = model * box5; box6 = model * box6; box7 = model * box7;

    //minx > right || miny > top || maxx < left || maxy < bottom 则出了屏幕.
    if (box0.x > right || box0.y > top || box1.x > right || box1.y < bottom || box2.x < left || box2.y < bottom ||
        box3.x < left  || box3.y > top || box4.x > right || box4.y > top    || box5.x > right|| box5.y < bottom ||
        box6.x < left  || box6.y < bottom || box7.x < left || box7.y > top)
    { 
//         itr_pos = find(needed_load_.begin(),needed_load_.end(),itr_obj_path->second.file_path);
//         if (itr_pos != needed_load_.end()) //该obj不再需要.
//         {
//           itr_obj_path->second.has_load = false;
//           needed_load_.erase(itr_pos);
//         }
      itr_obj_path->second.needed_load = false;

    }
    else  //在屏幕内.需要显示出来
    {
//       itr_pos = find(needed_load_.begin(),needed_load_.end(),itr_obj_path->second.file_path);
//       if (itr_pos != needed_load_.end())  //已经在屏幕内了,但是已经加载过,不需要重新加载;
//       {
//         itr_obj_path->second.has_load = true;
//       }
//       else    //已经在屏幕内了, 但是没有加载;需要重新加载;
//       {
//         itr_obj_path->second.has_load = false;
//         needed_load_.push_back(itr_obj_path->second.file_path);
//       }
      itr_obj_path->second.needed_load = true;

    }//else  //在屏幕内.需要显示出来
  }//for (int i = 0; i != itr_obj_path_count; ++i,++itr_obj_path)

  Unlock();

}

void OBJLoader::GetLoadedMaterial(RenderOBJ &robj, OBJRenderer &objrenderer)
{
  std::string str_file;
  Lock();
    for (int i = 0; i < vecOBJData_.size(); ++i)
    {
      if (!vecOBJData_[i].has_load())
        break;
      robj.init();
      robj.LoadOBJ_OPENGL(vecOBJData_[i]);
      objrenderer.LoadData(robj);
      vecOBJData_[i].set_has_render(true);
    }
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
     file_head.lod = i;
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
     // itr_obj_path->second.data_finsh = false;
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


