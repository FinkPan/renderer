#ifndef _HS_RENDER_OBJRENDERER_OBJLOADER_HPP_
#define _HS_RENDER_OBJRENDERER_OBJLOADER_HPP_

#include <map>
#include <string>
#include <thread>

#include "hs_renderer/renderer/OBJData.hpp"
#include "hs_renderer/renderer/ViewTransformer.hpp"

class OBJLoader
{
public:
  OBJLoader();
  ~OBJLoader();

  void LoadOBJ(std::string file_path);
  void Start();
  void Stop();

private:
  void Work();

  std::multimap<int, OBJData> m_lod_OBJ_;

  float box_[6]; //АќЮЇКа,

  std::thread working_thread_;

  int keep_working_;
  int keep_loading_;

};

#endif