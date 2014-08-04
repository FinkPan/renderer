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
#include "hs_renderer/renderer/OBJRenderer.hpp"

class OBJLoader
{
  struct FileHead
  {
    FileHead():lod(0),data_finsh(false),has_load(false),needed_load(false),has_render(false) {}
    ~FileHead(){}
     std::string file_path;
     int lod;
     float box[6]; //��Χ�� minx, miny, minz, maxx, maxy, maxz;
     bool data_finsh;
     bool has_load;
     bool needed_load;
     bool has_render;
  };
public:
  OBJLoader();
  ~OBJLoader();

  void LoadOBJ(const std::string& file_path);
  void InitializeViewTransformer(ViewTransformer& view_transformer);
  void UpdateLoadingOBJ(const ViewTransformer& view_transformer);
  void GetLoadedMaterial(RenderOBJ &robj, OBJRenderer &objrenderer);
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
  float box_[6]; //��Χ�� minx, miny, minz, maxx, maxy, maxz;
  int lod_min_;
  int lod_max_;
  int current_lod_;
  int prefix_lod_;

  std::thread working_thread_;
  std::thread calc_box_thread_;
  std::mutex mutex_;

  unsigned int obj_count_;
  int keep_working_;
  int keep_loading_;
  bool box_finish_;

};

#endif