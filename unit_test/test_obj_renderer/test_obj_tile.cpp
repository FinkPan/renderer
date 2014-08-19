//内存泄漏检测
#define _CRTDBG_MAP_ALLOC
#include "stdlib.h"
#include "crtdbg.h"

#include <string>
#include <cstdlib> //int rand()

#include "gtest/gtest.h"

#include "hs_renderer/obj_renderer/obj_tile.hpp"

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

typedef hs::render::obj_render::OBJTile<Scalar> OBJTile;

  const PScalar& TestConstBoundingBox(const OBJTile& v) 
  {
     return v.BoundingBox();
  }


  TEST(TestOBJTile, OBJTileTest)
  {
    EnableMemLeakCheck();
    //_CrtSetBreakAlloc(739);

    OBJTile obj_tile;

    obj_tile.SetLoaded();
    ASSERT_EQ(true,obj_tile.IsLoaded());
    obj_tile.SetUnloaded();
    ASSERT_EQ(false,obj_tile.IsLoaded());

    obj_tile.SetLod( 20 );
    ASSERT_EQ(20, obj_tile.GetLod());
    int lod = 30;
    obj_tile.SetLod() = lod;
    ASSERT_EQ(30, obj_tile.GetLod());

    Scalar* bounding_box3 = new Scalar[6];
    bounding_box3[0] = 31.5f;  bounding_box3[1] = 4.7f; bounding_box3[2] = 36.5f;
    bounding_box3[3] =-4.85f;  bounding_box3[4] =-6.47f;bounding_box3[5] = -2.9f;
    //test      PScalar& BoundingBox() { return bounding_box_;}
    memcpy(obj_tile.BoundingBox(),bounding_box3,6*sizeof(Scalar));
    Scalar bounding_box_out2[6];
    memcpy(bounding_box_out2,obj_tile.BoundingBox(),6*sizeof(Scalar));
    for (int i = 0; i != 6; ++i)
    {
      ASSERT_EQ(31.5f,bounding_box_out2[0]); ASSERT_EQ(4.7f,bounding_box_out2[1]); 
      ASSERT_EQ(36.5f,bounding_box_out2[2]); ASSERT_EQ(-4.85f,bounding_box_out2[3]);
      ASSERT_EQ(-6.47f,bounding_box_out2[4]);ASSERT_EQ(-2.9f,bounding_box_out2[5]);
      //test    const PScalar BoundingBox() const { return bounding_box_;}
      ASSERT_EQ(bounding_box3[i], TestConstBoundingBox(obj_tile)[i]);
    }
    delete [] bounding_box3;

     //test 顶点数据
    size_t vertex_size = 2562568;
    obj_tile.VertexSize() = vertex_size;
    ASSERT_EQ(vertex_size, obj_tile.VertexSize());

    static int test_array_size = 100;
    PScalar vertex_data = new Scalar[test_array_size];
    PScalar vd = new Scalar[test_array_size];
    for (int i = 0; i < test_array_size; ++i)
    {
      vd[i] = (Scalar)(i * 0.1);
    }
    if (true)
    {
      OBJTile obj_temp;
      obj_temp.VertexSize() = test_array_size;
      memcpy(obj_temp.VertexData(),vd,test_array_size*sizeof(Scalar));
      memcpy(vertex_data,obj_temp.VertexData(),test_array_size*sizeof(Scalar));
    }

    //test UV数据
    PScalar uv_data = new Scalar[test_array_size];
    obj_tile.UVSize() = test_array_size;
    ASSERT_EQ(test_array_size, obj_tile.UVSize());
    memcpy(obj_tile.UVData(),vd,test_array_size*sizeof(Scalar));
    memcpy(uv_data,obj_tile.UVData(),test_array_size*sizeof(Scalar));

    //test Index数据
    PIndexType index_data = new IndexType[test_array_size];
    PIndexType id = new IndexType[test_array_size];
    for (int i = 0; i < test_array_size; ++i)
    {
      id[i] = IndexType(i * 2);
    }
    obj_tile.IndexSize() = test_array_size;
    ASSERT_EQ(test_array_size,obj_tile.IndexSize());
    memcpy(obj_tile.UVData(),id,test_array_size*sizeof(IndexType));
    memcpy(index_data,obj_tile.UVData(),test_array_size*sizeof(IndexType));

    //test 顶点,UV,index数据
    for (int i = 0; i < test_array_size; ++i)
    {
      ASSERT_EQ(vd[i], vertex_data[i]);
      ASSERT_EQ(vd[i], uv_data[i]);
      ASSERT_EQ(id[i], index_data[i]);
    }

    //test image数据
    obj_tile.ImageWidth() = 2485;
    obj_tile.ImageHeight() = 584;
    ASSERT_EQ(2485,obj_tile.ImageWidth()); ASSERT_EQ(584,obj_tile.ImageHeight());
    PImageType image_data = new ImageType[2485*584*3];
    PImageType image_d = new ImageType[2485*584*3];
    for (int i = 0; i < 2485*584*3; ++i)
    {
      image_d[i] = rand() % 255;
    }
    memcpy(obj_tile.ImageData(),image_d,2485*584*3*sizeof(ImageType));
    memcpy(image_data,obj_tile.ImageData(),2485*584*3*sizeof(ImageType));

    for (int i = 0; i < 2485*584*3; ++i)
    {
      ASSERT_EQ(image_d[i], image_data[i]);
    }


    delete [] vd;
    delete [] vertex_data;
    delete [] uv_data;
    delete [] index_data;
    delete [] id;
    delete [] image_data;
    delete [] image_d;
  }
}