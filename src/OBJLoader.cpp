#include "hs_renderer/renderer/OBJLoader.hpp"

OBJLoader::OBJLoader()
{
}

OBJLoader::~OBJLoader()
{
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

void OBJLoader::Work()
{
  while (keep_working_)
  {
  }
}

void OBJLoader::LoadOBJ(std::string file_path)
{

}

