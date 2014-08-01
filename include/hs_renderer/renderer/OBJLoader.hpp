#ifndef _HS_RENDER_OBJRENDERER_OBJLOADER_HPP_
#define _HS_RENDER_OBJRENDERER_OBJLOADER_HPP_

#include <map>
#include <string>
#include <thread>
#include <vector>
#include <mutex>

#include "hs_renderer/renderer/ViewTransformer.hpp"
#include "hs_renderer/renderer/RenderOBJ.hpp"
#include "hs_renderer/renderer/OBJData.hpp"

class OBJLoader
{
  struct FileHead
  {
    FileHead():data_finsh(false),has_load(false),needed_load(false) {}
    ~FileHead(){}
     std::string file_path;
     float box[6]; //АќЮЇКа minx, miny, minz, maxx, maxy, maxz;
     bool data_finsh;
     bool has_load;
     bool needed_load;
  };
public:
  OBJLoader();
  ~OBJLoader();

  void LoadOBJ(const std::string& file_path);
  void InitializeViewTransformer(ViewTransformer& view_transformer);
  void UpdateLoadingOBJ(const ViewTransformer& view_transformer);
  void GetLoadedMaterial(RenderOBJ &robj);
  void Start();
  void Stop();

private:
  inline void Lock() { mutex_.lock(); }
  inline void Unlock() { mutex_.unlock();}
  void Load(const std::string &boj_path);
  void Work();
  void AnalysisFilePath(std::string &file_name, int &lod_min, int &lod_max);
  void CalculateBox();

  std::vector<OBJData> vecOBJData_;
  std::multimap<int, FileHead> m_FileInfo_;
  float box_[6]; //АќЮЇКа minx, miny, minz, maxx, maxy, maxz;
  int lod_min_;
  int lod_max_;
  int current_lod_;
  std::string current_obj_;
  //std::vector<std::string> needed_load_;

  std::thread working_thread_;
  std::thread calc_box_thread_;
  std::mutex mutex_;

  int keep_working_;
  int keep_loading_;
  bool box_finish_;

};

#endif