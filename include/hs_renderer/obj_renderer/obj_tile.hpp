#ifndef _HS_RENDERER_OBJ_RENDER_OBJ_TILE_HPP_
#define _HS_RENDERER_OBJ_RENDER_OBJ_TILE_HPP_

#include <vector>
#include <algorithm>

namespace hs
{
namespace renderer
{
namespace render
{

template <typename _Scalar> class OBJTile;

template <typename _Scalar>
class OBJTile
{
public:
  typedef _Scalar Scalar;
  typedef Scalar* PScalar;
  typedef unsigned int IndexType;
  typedef IndexType* PIndexType;
  typedef unsigned char ImageType;
  typedef ImageType* PImageType;
  typedef OBJTile<Scalar> Self;

public:
  OBJTile()
    :is_loaded_(false),
    lod_(-1),
    vertex_size_(0),
    vertex_data_(nullptr),
    uv_size_(0),
    uv_data_(nullptr),
    index_size_(0),
    index_data_(nullptr),
    image_width_(0),
    image_height_(0),
    image_data_(nullptr)
  {
    bounding_box_ = new Scalar[6];
  }
  ~OBJTile()
  {
    if (bounding_box_ != nullptr)
    {
      delete [] bounding_box_;
    }
    if (vertex_data_ != nullptr)
    {
      delete [] vertex_data_;
    }
    if (uv_data_ != nullptr)
    {
      delete [] uv_data_;
    }
    if (index_data_ != nullptr)
    {
      delete [] index_data_;
    }
    if (image_data_ != nullptr)
    {
      delete [] image_data_;
    }
  }

  void SetLoaded(){ is_loaded_ = true;}
  void SetUnloaded(){  is_loaded_ = false;}
  bool IsLoaded() const{   return is_loaded_;}

  //����lod
  void SetLod(int lod) { lod_ = lod; }
  int& SetLod() {return lod_;} 
  int GetLod() const { return lod_; }

  //���ð�Χ�б�����6��Scalar
  const PScalar& BoundingBox() const { return bounding_box_;}
  PScalar& BoundingBox() { return bounding_box_;}

  //���ö�������
  const size_t& VertexSize() const { return vertex_size_; }
  size_t& VertexSize() { return vertex_size_; }
  const PScalar& VertexData() const {return vertex_data_;}
  PScalar& VertexData()
  {
    if (vertex_size_ == 0)
    {
      std::cout << "�޷����䶥������ռ�,������VertexSize���ö��������С!\n";
    }
    if (vertex_data_ == nullptr)
    {
      vertex_data_ = new Scalar[vertex_size_];
    }
    return vertex_data_; 
  }

  //����UV����
  const size_t& UVSize() const { return uv_size_; }
  size_t& UVSize() { return uv_size_; }
  const PScalar& UVData() const { return uv_data_; }
  PScalar& UVData() 
  {
    if (uv_size_ == 0)
    {
      std::cout << "�޷�����UV����ռ�,������SetUVSize����UV�����С!\n";
    }
    if (uv_data_ == nullptr)
    {
      uv_data_ = new Scalar[uv_size_];
    }
    return uv_data_;
  }

  //����index����
  const size_t& IndexSize() const { return index_size_; }
  size_t& IndexSize() { return index_size_; }
  const PIndexType& IndexData() const { return index_data_; }
  PIndexType& IndexData() 
  {
    if (index_size_ == 0)
    {
      std::cout << "�޷�����index�ռ�,������SetIndexSize����index��С!\n";
    }
    if (index_data_ == nullptr)
    {
      index_data_ = new IndexType[index_size_];
    }
    return index_data_;
  }

  //����ͼƬ����,ֻ֧��3ͨ��ͼƬ
  const int& ImageWidth() const { return image_width_;}
  int& ImageWidth() { return image_width_; }
  const int& ImageHeight() const { return image_height_; }
  int& ImageHeight() { return image_height_; }
  const PImageType& ImageData() const { return image_data_; }
  PImageType& ImageData() 
  {
    if (image_width_ == 0 || image_height_ == 0)
    {
      std::cout << "ֻ֧��3ͨ��,��δ֪ͼ���С,�޷�����ռ�!\n";
    }
    if (image_data_ == nullptr)
    {
      image_data_ = new ImageType[image_width_ * image_height_ * 3];
    }
    return image_data_;
  }

private:
  bool is_loaded_;
  int lod_;
  PScalar bounding_box_;
  size_t vertex_size_;
  PScalar vertex_data_;
  size_t uv_size_;
  PScalar uv_data_;
  size_t index_size_;
  PIndexType index_data_;

  int image_width_;
  int image_height_;
  PImageType image_data_;

};

}//namespace obj_render
}//namespace render
}//namespace hs
#endif