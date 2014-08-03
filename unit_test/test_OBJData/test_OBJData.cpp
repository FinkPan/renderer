//内存泄漏检测
#define _CRTDBG_MAP_ALLOC
#include "stdlib.h"
#include "crtdbg.h"

#include <gtest/gtest.h>

#include <string>
#include <fstream>

#include "hs_renderer/renderer/OBJData.hpp"
#include "hs_image_io/whole_io/image_data.hpp"
#include "hs_image_io/whole_io/image_io.hpp"


namespace
{
  inline void EnableMemLeakCheck()
  {
    _CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);
  }
#ifdef _DEBUG
#define new   new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif


  int GenerateImageData(const int &image_width, const int &image_height, const int &image_channel, unsigned char *image_data)
  {
    if (image_data == nullptr)
    {
      std::cout << "image_data没有分配内存!\n";
      return -1;
    }
    for (int row = 0; row < image_height/2; ++row)  //红色
    {
      for (int col = 0; col < image_width/2; ++col)
      {
        image_data[ row*image_width*image_channel + col*image_channel + 0 ] = 255;
        image_data[ row*image_width*image_channel + col*image_channel + 1 ] = 0;
        image_data[ row*image_width*image_channel + col*image_channel + 2 ] = 0;
      }
    }
    for (int row = 0; row < image_height/2; ++row)  //绿色
    {
      for (int col = image_width/2; col < image_width; ++col)
      {
        image_data[ row*image_width*image_channel + col*image_channel + 0 ] = 0;
        image_data[ row*image_width*image_channel + col*image_channel + 1 ] = 255;
        image_data[ row*image_width*image_channel + col*image_channel + 2 ] = 0;
      }
    }
    for (int row = image_height/2; row < image_height; ++row)  //蓝色
    {
      for (int col = 0; col < image_width/2; ++col)
      {
        image_data[ row*image_width*image_channel + col*image_channel + 0 ] = 0;
        image_data[ row*image_width*image_channel + col*image_channel + 1 ] = 0;
        image_data[ row*image_width*image_channel + col*image_channel + 2 ] = 255;
      }
    }
    for (int row = image_height/2; row < image_height; ++row)  //红+蓝色
    {
      for (int col = image_width/2; col < image_width; ++col)
      {
        image_data[ row*image_width*image_channel + col*image_channel + 0 ] = 255;
        image_data[ row*image_width*image_channel + col*image_channel + 1 ] = 0;
        image_data[ row*image_width*image_channel + col*image_channel + 2 ] = 255;
      }
    }
    return -1;
  }

  int GenerateData()
  {
     std::string out_file_path = "lod_17.bin";
     std::ofstream out_file(out_file_path, std::ios_base::binary);
     if (!out_file.is_open())
     {
       return -1;
     }
     int lod = 17;
     out_file.write((char*)&lod, sizeof(int));
     const float  box[6] = // minx, miny, minz, maxx, maxy, maxz;
     {
       -1.1f,-1.0f,-1.0f,  1.5f,1.5f,1.5f 
     };
     out_file.write((char*)box, 6*sizeof(float));

     const float vertex_data[] = 
     {
       1.0f, 0.0f, 0.0f, 1.0f,
       0.0f, 1.0f, 0.0f, 1.0f,
       0.0f, 0.0f, 1.0f, 1.0f,

      -1.0f, 0.0f, 0.0f, 1.0f,
       0.0f, 1.0f, 0.0f, 1.0f,
       0.0f, 0.0f, 1.0f, 1.0f
     };
     size_t vertex_size = 24;
     out_file.write((char*)&vertex_size,sizeof(size_t));
     out_file.write((char*)vertex_data, vertex_size*sizeof(float));

     const float uvcoord_data[] = 
     {
       0.0f, 0.0f,  1.0f,
       0.0f, 0.5f,  1.0f,
       0.5f, 0.0f,  1.0f,

       0.0f, 0.5f, 1.0f,
       0.5f, 0.5f, 1.0f,
       0.0f, 1.0f, 1.0f
     };
     size_t uvcoord_size = 18;
     out_file.write((char*)&uvcoord_size,sizeof(size_t));
     out_file.write((char*)uvcoord_data, uvcoord_size*sizeof(float));

     const unsigned int index_data[] = 
     {
       0,1,2,
       3,4,5
     };
     size_t index_size = 6;
     out_file.write((char*)&index_size,sizeof(size_t));
     out_file.write((char*)index_data, index_size*sizeof(unsigned int));

    //生成贴图
     int image_width = 20, image_height = 20, image_channel = 3;
     unsigned char* image_data = new unsigned char[ image_width * image_height * image_channel ];
     GenerateImageData(image_width,image_height,image_channel,image_data);
     out_file.write((char*)&image_width, sizeof(int));
     out_file.write((char*)&image_height, sizeof(int));
     out_file.write((char*)image_data, image_width*image_height*image_channel*sizeof(unsigned char));
     out_file.close();

     delete [] image_data;
     return 0;
  }

  bool CompareData(const int &image_width, const int &image_height, const int &image_channel, const unsigned char* image_data)
  {
    if (image_data == nullptr)
    {
      std::cout << "image_data没有分配内存!\n";
      return false;
    }
    bool a = false, b = false, c = false, d = false;
    for (int row = 0; row < image_height/2; ++row)  //红色
    {
      for (int col = 0; col < image_width/2; ++col)
      {
        if( image_data[ row*image_width*image_channel + col*image_channel + 0 ] == 255 &&
            image_data[ row*image_width*image_channel + col*image_channel + 1 ] == 0 &&
            image_data[ row*image_width*image_channel + col*image_channel + 2 ] == 0 )
          a = true;
        else
          a = false;
      }
    }
    for (int row = 0; row < image_height/2; ++row)  //绿色
    {
      for (int col = image_width/2; col < image_width; ++col)
      {
        if(image_data[ row*image_width*image_channel + col*image_channel + 0 ] == 0 &&
           image_data[ row*image_width*image_channel + col*image_channel + 1 ] == 255 &&
           image_data[ row*image_width*image_channel + col*image_channel + 2 ] == 0)
          b = true;
        else
          b = false;

      }
    }
    for (int row = image_height/2; row < image_height; ++row)  //蓝色
    {
      for (int col = 0; col < image_width/2; ++col)
      {
        if( image_data[ row*image_width*image_channel + col*image_channel + 0 ] == 0 &&
            image_data[ row*image_width*image_channel + col*image_channel + 1 ] == 0 &&
            image_data[ row*image_width*image_channel + col*image_channel + 2 ] == 255)
            c = true;
        else
        c = false;

      }
    }
    for (int row = image_height/2; row < image_height; ++row)  //红+蓝色
    {
      for (int col = image_width/2; col < image_width; ++col)
      {
        if( image_data[ row*image_width*image_channel + col*image_channel + 0 ] == 255 &&
            image_data[ row*image_width*image_channel + col*image_channel + 1 ] == 0 &&
            image_data[ row*image_width*image_channel + col*image_channel + 2 ] == 255)
            d = true;
        else
        d = false;
      }
    }
    return a && b && c && d;

  }

  TEST(TestOBJData, GenerateImageData)
  {
    int image_width = 500; int image_height = 500; int image_channel = 3;
    //unsigned char* image_data = new unsigned char[ image_width * image_height * image_channel ];
    hs::imgio::whole::ImageData image_data_;
    image_data_.CreateImage(image_width,image_height,image_channel);
    GenerateImageData(image_width, image_height, image_channel, image_data_.GetBuffer());
    hs::imgio::whole::ImageIO image_io;
    image_io.SaveImage("temp.png", image_data_);

  }

  TEST(TestOBJData, GenerateData)
  {

    EnableMemLeakCheck();
    //_CrtSetBreakAlloc(687);
    //   if (argc ==1)
    //   {
    //     std::cout << "Usage: hs_readOBJ.exe obj_name\n";
    //     return -1;
    //   }

     EXPECT_EQ(0, GenerateData());  //生成并写入文件;

     std::string out_file_path = "lod_17.bin";
     std::ifstream in_file(out_file_path, std::ios_base::binary);
     if (!in_file.is_open())
     {
       std::cout<< "无法打开文件!";
       return;
     }

     int lod = 0;
     in_file.read((char*)&lod,sizeof(int));
     EXPECT_EQ(17, lod);

     float  box[6]; //       -1.1f,-1.0f,-1.0f,  1.5f,1.5f,1.5f 
     in_file.read((char*)box, 6*sizeof(float));
     EXPECT_EQ(-1.1f, box[0]); EXPECT_EQ(-1.0f, box[1]);  EXPECT_EQ(-1.0f, box[2]); 
     EXPECT_EQ(1.5f, box[3]);  EXPECT_EQ(1.5f, box[4]);   EXPECT_EQ(1.5f, box[5]);

     size_t vertex_size = 0;
     in_file.read((char*)&vertex_size, sizeof(size_t));
     EXPECT_EQ(24, vertex_size);
     float* vertex_data = new float[vertex_size];
     in_file.read((char*)vertex_data, vertex_size*sizeof(float));
     EXPECT_EQ(1.0f, vertex_data[0]); EXPECT_EQ(0.0f, vertex_data[1]);  EXPECT_EQ(0.0f, vertex_data[2]); EXPECT_EQ(1.0f, vertex_data[3]);
     EXPECT_EQ(0.0f, vertex_data[4]); EXPECT_EQ(1.0f, vertex_data[5]);  EXPECT_EQ(0.0f, vertex_data[6]); EXPECT_EQ(1.0f, vertex_data[7]);
     EXPECT_EQ(0.0f, vertex_data[8]); EXPECT_EQ(0.0f, vertex_data[9]);  EXPECT_EQ(1.0f, vertex_data[10]); EXPECT_EQ(1.0f, vertex_data[11]);

     EXPECT_EQ(-1.0f, vertex_data[12]); EXPECT_EQ(0.0f, vertex_data[13]);  EXPECT_EQ(0.0f, vertex_data[14]); EXPECT_EQ(1.0f, vertex_data[15]);
     EXPECT_EQ(0.0f, vertex_data[16]); EXPECT_EQ(1.0f, vertex_data[17]);  EXPECT_EQ(0.0f, vertex_data[18]); EXPECT_EQ(1.0f, vertex_data[19]);
     EXPECT_EQ(0.0f, vertex_data[20]); EXPECT_EQ(0.0f, vertex_data[21]);  EXPECT_EQ(1.0f, vertex_data[22]); EXPECT_EQ(1.0f, vertex_data[23]);

     size_t uvcoord_size = 0;
     in_file.read((char*)&uvcoord_size, sizeof(size_t));
     EXPECT_EQ(18, uvcoord_size);
     float* uvcoord_data = new float[uvcoord_size];
     in_file.read((char*)uvcoord_data, uvcoord_size*sizeof(float));
     EXPECT_EQ(0.0f, uvcoord_data[0]); EXPECT_EQ(0.0f, uvcoord_data[1]);  EXPECT_EQ(1.0f, uvcoord_data[2]);
     EXPECT_EQ(0.0f, uvcoord_data[3]); EXPECT_EQ(1.0f, uvcoord_data[4]);  EXPECT_EQ(1.0f, uvcoord_data[5]);
     EXPECT_EQ(1.0f, uvcoord_data[6]); EXPECT_EQ(1.0f, uvcoord_data[7]);  EXPECT_EQ(1.0f, uvcoord_data[8]);

     EXPECT_EQ(0.0f, uvcoord_data[9]); EXPECT_EQ(0.0f, uvcoord_data[10]);  EXPECT_EQ(1.0f, uvcoord_data[11]);
     EXPECT_EQ(1.0f, uvcoord_data[12]); EXPECT_EQ(0.0f, uvcoord_data[13]);  EXPECT_EQ(1.0f, uvcoord_data[14]);
     EXPECT_EQ(1.0f, uvcoord_data[15]); EXPECT_EQ(1.0f, uvcoord_data[16]);  EXPECT_EQ(1.0f, uvcoord_data[17]);

     size_t index_size = 0;
     in_file.read((char*)&index_size, sizeof(size_t));
     EXPECT_EQ(6,index_size);
     unsigned int* index_data = new unsigned int[index_size];
     in_file.read((char*)index_data,index_size * sizeof(unsigned int));
     EXPECT_EQ(0, index_data[0]); EXPECT_EQ(1, index_data[1]);  EXPECT_EQ(2, index_data[2]);
     EXPECT_EQ(3, index_data[3]); EXPECT_EQ(4, index_data[4]);  EXPECT_EQ(5, index_data[5]);

     //读取图片
     int image_width = 0, image_height = 0, image_channel = 3;
     in_file.read((char*)&image_width, sizeof(int));
     in_file.read((char*)&image_height,sizeof(int));
     EXPECT_EQ(20,image_width); EXPECT_EQ(20,image_height);
     unsigned char* image_data = new unsigned char[image_width*image_height*image_channel];
     in_file.read((char*)image_data, image_width*image_height*image_channel*sizeof(unsigned char));
     EXPECT_EQ(true, CompareData(image_width,image_height,image_channel,image_data));

     in_file.close();
     delete [] vertex_data;
     delete [] uvcoord_data;
     delete [] index_data;
     delete [] image_data;

  }

  TEST(TestOBJData, LoadOBJ)
  {
    EnableMemLeakCheck();
    //_CrtSetBreakAlloc(687);
    //   if (argc ==1)
    //   {
    //     std::cout << "Usage: hs_readOBJ.exe obj_name\n";
    //     return -1;
    //   }

    EXPECT_EQ(0, GenerateData());  //生成并写入文件;
    std::string file_path = "lod_17.bin";
    OBJData obj_data;
    obj_data.LoadOBJ(file_path);

    EXPECT_EQ(20, obj_data.image_width());
    EXPECT_EQ(20, obj_data.image_height());
    float* box = new float[6];
    box = obj_data.box();   //浅拷贝不需要delete;
    EXPECT_EQ(-1.1f, box[0]); EXPECT_EQ(-1.0f, box[1]);  EXPECT_EQ(-1.0f, box[2]); 
    EXPECT_EQ(1.5f, box[3]);  EXPECT_EQ(1.5f, box[4]);   EXPECT_EQ(1.5f, box[5]);
    //delete [] box; 浅拷贝不需要delete;

    EXPECT_EQ(24, obj_data.vertex_size());
    float* vertex_data = new float[obj_data.vertex_size()];
    vertex_data = obj_data.vertex_data(); // 浅拷贝不需要delete;

    EXPECT_EQ(1.0f, vertex_data[0]); EXPECT_EQ(0.0f, vertex_data[1]);  EXPECT_EQ(0.0f, vertex_data[2]); EXPECT_EQ(1.0f, vertex_data[3]);
    EXPECT_EQ(0.0f, vertex_data[4]); EXPECT_EQ(1.0f, vertex_data[5]);  EXPECT_EQ(0.0f, vertex_data[6]); EXPECT_EQ(1.0f, vertex_data[7]);
    EXPECT_EQ(0.0f, vertex_data[8]); EXPECT_EQ(0.0f, vertex_data[9]);  EXPECT_EQ(1.0f, vertex_data[10]); EXPECT_EQ(1.0f, vertex_data[11]);

    EXPECT_EQ(-1.0f, vertex_data[12]); EXPECT_EQ(0.0f, vertex_data[13]);  EXPECT_EQ(0.0f, vertex_data[14]); EXPECT_EQ(1.0f, vertex_data[15]);
    EXPECT_EQ(0.0f, vertex_data[16]); EXPECT_EQ(1.0f, vertex_data[17]);  EXPECT_EQ(0.0f, vertex_data[18]); EXPECT_EQ(1.0f, vertex_data[19]);
    EXPECT_EQ(0.0f, vertex_data[20]); EXPECT_EQ(0.0f, vertex_data[21]);  EXPECT_EQ(1.0f, vertex_data[22]); EXPECT_EQ(1.0f, vertex_data[23]);

    
  }
}