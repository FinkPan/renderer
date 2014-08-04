#include "hs_renderer/renderer/OBJLoader.hpp"
#include <fstream>
#include <iostream>
#include <sstream> 
#include <algorithm>

#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>


OBJLoader::OBJLoader() :
  lod_min_(0),lod_max_(0),box_finish_(false),obj_count_(0)
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
    //���lod�ı�,�����vecOBJData_;
    if (!vecOBJData_.empty())
    {
      if (vecOBJData_[0].lod() != current_lod_)
      {
        std::cout << "lod�ı�!!!current_lod_= " <<  current_lod_ << "\n";
        auto itr_obj_path1 = m_FileInfo_.find(vecOBJData_[0].lod());
        size_t itr_obj_path_count1 = m_FileInfo_.count(current_lod_);
        for (int i = 0; i != itr_obj_path_count1; ++i,++itr_obj_path1)
        {
          itr_obj_path1->second.has_load = false;
        }
        vecOBJData_.clear();
      }
    }

    //Ĭ�ϼ�����߲��һ��obj    
    auto itr_obj_path = m_FileInfo_.find(current_lod_);
    size_t itr_obj_path_count = m_FileInfo_.count(current_lod_);
    for (int i = 0; i != itr_obj_path_count; ++i,++itr_obj_path)
    {

      if (itr_obj_path->second.needed_load == true) //��obj��Ҫ����
      {

        if (itr_obj_path->second.has_load == false) //��objû�м���
        {
          std::cout << "objû�м���,��Ҫ����!!!\n";

          obj_data.LoadOBJ(itr_obj_path->second.file_path);
          itr_obj_path->second.has_load = true;
          vecOBJData_.push_back(obj_data);
          std::cout << "Work:::vecOBJData_.size() = " << vecOBJData_.size() << "\n";

        }
      }
      else   //����Ҫ����,����Ѿ�������ɾ����obj
      {
        for (auto itr_pos = vecOBJData_.begin(); itr_pos != vecOBJData_.end(); ++itr_pos)
        {

          if (itr_pos->getfilename() == itr_obj_path->second.file_path)
          {
            std::cout << "obj��Ҫɾ��!!!\n";
            itr_obj_path->second.has_load = false;
            std::cout << "ɾ��" << itr_obj_path->second.file_path << "\n";
            vecOBJData_.erase(itr_pos);
            if (vecOBJData_.empty())
            {
              break;
            }
            itr_pos = vecOBJData_.begin();
          }                         
        }
      }
    }
    Unlock();

  }//  while (keep_working_)

}

//�ļ���ʽ:
//14 20
//E:\workspace\obj\tianyang_gonglian\Tile_+003_+004
//...
void OBJLoader::LoadOBJ(const std::string& file_path)
{
  std::string file_name;
  int lod_min = 0, lod_max = 0;
  std::ifstream list_file(file_path);
  Lock();
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
  Unlock();
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

void FindMinMax(const float &f0,const float &f1,const float &f2,const float &f3,
                const float &f4,const float &f5,const float &f6,const float &f7,
                float &min, float &max)
{
   float val[] = {f0,f1,f2,f3,f4,f5,f6,f7};
   min = *std::min_element(val,val+8);
   max = *std::max_element(val,val+8);
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
  float minx = 0.0f, maxx = 0.0f, miny = 0.0f, maxy = 0.0f;
  int current_lod = vt.lod(); 

  Lock();
  if (current_lod > lod_max_)
  {
    current_lod_ = lod_max_;
  }
  else if(current_lod < lod_min_)
  {
    current_lod_ = lod_min_;
  }
  else
  {
    current_lod_ = current_lod;
  }

  auto itr_obj_path = m_FileInfo_.find(current_lod_);
  size_t itr_obj_path_count = m_FileInfo_.count(current_lod_);
  std::cout << "itr_obj_path_count = " << itr_obj_path_count << "\n";

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
    //��仯����model�������;
    box0 = model * box0; box1 = model * box1; box2 = model * box2; box3 = model * box3; box4 = model * box4;
    box5 = model * box5; box6 = model * box6; box7 = model * box7;
    int a = 1, b = 2, c = 3, d = 4;

     FindMinMax(box0.x, box1.x, box2.x, box3.x, box4.x, box5.x, box6.x, box7.x,minx,maxx);
     FindMinMax(box0.y, box1.y, box2.y, box3.y, box4.y, box5.y, box6.y, box7.y,miny,maxy);


     std::cout << "\nminx= " << minx << " >? right= " << right << "\n";
     std::cout << "miny= " << miny << " >? top= " << top << "\n";
     std::cout << "maxx= " << maxx << " <? left= " << left << "\n";
     std::cout << "maxy= " << maxy << " <? bottom= " << bottom << "\n";


    //minx > right || miny > top || maxx < left || maxy < bottom �������Ļ.
    if (minx > right || miny > top || maxx < left || maxy < bottom)
    { 
      std::cout << "obj���߽���\n";
      itr_obj_path->second.needed_load = false;
    }
    else  //����Ļ��.��Ҫ��ʾ����
    {
      std::cout << "obj����Ļ��.��Ҫ��ʾ����\n";
      itr_obj_path->second.needed_load = true;
    }//else  //����Ļ��.��Ҫ��ʾ����
  }//for (int i = 0; i != itr_obj_path_count; ++i,++itr_obj_path)

  std::cout << "obj_count_ = " << obj_count_ << "\n";

  Unlock();

}

void OBJLoader::GetLoadedMaterial(RenderOBJ &robj, OBJRenderer &objrenderer)
{
  std::string str_file;
 // Lock();
  std::cout << "vecOBJData_.size() = " << vecOBJData_.size() << "\n";
  if (vecOBJData_.empty())
  {
    std::cout << "vecOBJData_.empty() \n";
    return;
  }
  for (int i = 0; i < vecOBJData_.size(); ++i)
  {
    if (vecOBJData_[i].has_render())
      break;
    robj.LoadOBJ_OPENGL(vecOBJData_[i]);
    objrenderer.LoadData(robj);
    //vecOBJData_[i].set_has_render(true);
  }
  //Unlock();
}

void OBJLoader::AnalysisFilePath(std::string &file_name, int &lod_min, int &lod_max)
{
   std::string file_name_id;
   FileHead file_head;
   size_t itr_slash_pos = file_name.rfind("\\");  //�ֽ���ļ���  
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
     file_head.file_path  = file_name + "\\" + file_name_id + "_L" + str_lod + ".bin";
     m_FileInfo_.insert(std::make_pair(i,file_head));
   }
}

void OBJLoader::CalculateBox()
{
  std::thread::id main_thread_id = std::this_thread::get_id();
  std::cout << "CalculateBox = " << main_thread_id << "\n";
  std::ifstream obj_file_path;
  //Lock();
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
  //Unlock();
}


