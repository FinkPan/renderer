//内存泄漏检测
#define _CRTDBG_MAP_ALLOC
#include "stdlib.h"
#include "crtdbg.h"

#include <fstream>
#include <cstdlib> //int rand()

#include "gtest/gtest.h"

#include "hs_renderer/obj_renderer/obj_lod_tiles_loader_engine.hpp"

inline void EnableMemLeakCheck()
{
  _CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);
}
#ifdef _DEBUG
#define new   new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

namespace
{
  typedef float Scalar;
  typedef Scalar* PScalar;
  typedef unsigned int IndexType;
  typedef IndexType* PIndexType;
  typedef unsigned char ImageType;
  typedef ImageType* PImageType;

  typedef hs::renderer::render::OBJLODTilesLoaderEngine<Scalar>  OBJLODTilesLoaderEngine;

  void GenerateData(const std::string& obj_path, 
               int& lod, PScalar& bounding_box,
               size_t& vertex_size, PScalar& vertex_data,
               size_t& uv_size, PScalar& uv_data,
               size_t& index_size, PIndexType& index_data,
               int& image_width, int& image_height,
               PImageType& image_data
               )
  {
    std::ofstream out_obj_file(obj_path,std::ios_base::binary);
    if (!out_obj_file.is_open())
    {
      std::cout << "无法打开 " << obj_path << " 文件!\n";
      return;
    }
    out_obj_file.write((char*)&lod,sizeof(int));
    out_obj_file.write((char*)bounding_box,6*sizeof(Scalar));

    //生成顶点大小及数据
    out_obj_file.write((char*)&vertex_size,sizeof(size_t));
    out_obj_file.write((char*)vertex_data,vertex_size*sizeof(Scalar));

    //生成UV大小及数据
    out_obj_file.write((char*)&uv_size,sizeof(size_t));
    out_obj_file.write((char*)uv_data,uv_size*sizeof(Scalar));

    //生成index大小及数据
    out_obj_file.write((char*)&index_size,sizeof(size_t));
    out_obj_file.write((char*)index_data,index_size*sizeof(IndexType));

    //图片大小及数据
    out_obj_file.write((char*)&image_width, sizeof(int));
    out_obj_file.write((char*)&image_height,sizeof(int));
    out_obj_file.write((char*)image_data,image_width*image_height*3*sizeof(ImageType));
    out_obj_file.close();
  }


  TEST(OBJLODTilesLoaderEngine, ReadOBJFile)
  {
    EnableMemLeakCheck();
    //_CrtSetBreakAlloc(729);

    std::string obj_path = "test_obj_lod_tiles_loader_engine.bin";

    int lod = 26;
    PScalar bounding_box = new Scalar[6];
    bounding_box[0] = 5.4f; bounding_box[1] = 71.3f; bounding_box[2] = 58.4f;
    bounding_box[3] =-85.3f;bounding_box[4] = 4.6f;  bounding_box[5] = -726.7f;

    size_t vertex_size = 300;
    PScalar vertex_data = new Scalar[vertex_size];
    for (size_t i = 0; i < vertex_size; ++i)
    {
       vertex_data[i] = (Scalar) (i * 0.2);
    }
    size_t uv_size = 888;
    PScalar uv_data = new Scalar[uv_size];
    for (size_t i = 0; i < uv_size; ++i)
    {
      uv_data[i] = (Scalar) (i * 0.3);
    }
    size_t index_size = 246;
    PIndexType index_data = new IndexType[index_size];
    for (size_t i = 0; i < index_size; ++i)
    {
      index_data[i] = (IndexType) (i);
    }
    int image_width = 356;
    int image_height = 563;
    PImageType image_data = new ImageType[image_width * image_height * 3];
    for (size_t i = 0; i < image_width * image_height * 3; ++i)
    {
      image_data[i] = rand() % 255;
    }

    GenerateData(obj_path,lod,bounding_box,vertex_size,vertex_data,
                 uv_size,uv_data,index_size,index_data,
                 image_width,image_height,image_data);

   //OBJLODTilesLoaderEngine obj_engine(obj_path);
   //obj_engine.LoadTile()

   //比较lod
   ASSERT_EQ(lod, obj_engine.GetOBJData()->GetLod());
   for (int i = 0; i < 6; ++i)
   {
     ASSERT_EQ(bounding_box[i], obj_engine.GetOBJData()->BoundingBox()[i]);
   }
   ASSERT_EQ(vertex_size, obj_engine.GetOBJData()->VertexSize());
   for (size_t i = 0; i < vertex_size; ++i)
   {
     ASSERT_EQ(vertex_data[i], obj_engine.GetOBJData()->VertexData()[i]);
   }
   ASSERT_EQ(uv_size, obj_engine.GetOBJData()->UVSize());
   for (size_t i = 0; i < uv_size; ++i)
   {
     ASSERT_EQ(uv_data[i], obj_engine.GetOBJData()->UVData()[i]);
   }
   ASSERT_EQ(index_size,obj_engine.GetOBJData()->IndexSize());
   for (size_t i = 0; i < index_size; ++i)
   {
     ASSERT_EQ(index_data[i], obj_engine.GetOBJData()->IndexData()[i]);
   }
   ASSERT_EQ(image_width,obj_engine.GetOBJData()->ImageWidth());
   ASSERT_EQ(image_height,obj_engine.GetOBJData()->ImageHeight());
   for (size_t i = 0; i < image_width * image_height * 3; ++i)
   {
     ASSERT_EQ(image_data[i], obj_engine.GetOBJData()->ImageData()[i]);
   }
  }
  TEST(OBJLODTilesLoaderEngine, Open)
  {
    EnableMemLeakCheck();
    //_CrtSetBreakAlloc(729);

    std::string obj_list_path = "E:\\workspace\\obj\\tianyang_gonglian_bin\\List.txt";

    OBJLODTilesLoaderEngine obj_engine(obj_list_path);

  }

  TEST(OBJLODTilesLoaderEngine, LoadTile)
  {
    EnableMemLeakCheck();
    //_CrtSetBreakAlloc(729);

    std::string obj_list_path = "E:\\workspace\\obj\\tianyang_gonglian_bin\\List.txt";

    OBJLODTilesLoaderEngine obj_engine(obj_list_path);

  }

}