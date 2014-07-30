#include <iostream>
#include "hs_renderer/renderer/ViewTransformer.hpp"

ViewTransformer::ViewTransformer()
  :near_(-10000.0f), far_(10000.0f)
{
  Projection_ = glm::ortho(-3.0f, 3.0f, -3.0f,3.0f,-10000.0f,10000.0f);
  View_ = glm::lookAt(
    glm::vec3(0,0,1), //eye表示我们眼睛在"世界坐标系"中的位置
    glm::vec3(0,0,0), //center表示眼睛"看"的那个点的坐标,
    glm::vec3(0,1,0)  //up坐标表示观察者本身的方向
    );
  Model_ = glm::mat4(1.0f);
  MVP_ = Projection_ * View_ * Model_;
  axis_MVP_ = Projection_ * View_ * Model_;
}

ViewTransformer::~ViewTransformer()
{
}

const float* ViewTransformer::GetMVP()
{
  MVP_ = Projection_ * View_ * Model_;
  return glm::value_ptr(MVP_);
}
const float* ViewTransformer::GetPV()
{
  axis_MVP_ = Projection_ * View_;
  return glm::value_ptr(axis_MVP_);
}
const glm::mat4 ViewTransformer::GetModel() const
{
  return Model_;
}
void ViewTransformer::SetProjection(float left, float right, float bottom, float top, ProjectionMode projectionMode)
{
  left_ = left;    right_ = right;
  bottom_ = bottom;  top_ = top;
  CalculateBox();

  float resolution = (right - left)/viewport_width_;

  CalculateLod(resolution);
  std::cout << "lod= " << lod_ << "\n";

  if (projectionMode == PERSPECTIVE)
  {
    Projection_ = glm::frustum(left_,right_,bottom_,top_,near_, far_);
  }
  else
  {
    if (viewport_width_ <= viewport_height_)
      Projection_ = glm::ortho(viewport_left_,viewport_right_,viewport_bottom_*(GLfloat)viewport_height_/(GLfloat)viewport_width_,viewport_top_*(GLfloat)viewport_height_/(GLfloat)viewport_width_,near_, far_);
    else
      Projection_ = glm::ortho(viewport_left_*(GLfloat)viewport_width_/(GLfloat)viewport_height_,viewport_right_*(GLfloat)viewport_width_/(GLfloat)viewport_height_,viewport_bottom_,viewport_top_,near_, far_);
  }
}

void ViewTransformer::Update()
{
  CalculateBox();
}

void ViewTransformer::SetViewport(int x, int y, int width, int height)
{
  viewport_x_ = x;          viewport_y_ = y;
  viewport_width_ = width;  viewport_height_ = height;

  if (width <= height)
    Projection_ = glm::ortho(viewport_left_, viewport_right_,viewport_bottom_* (GLfloat)height/(GLfloat)width, viewport_top_ * (GLfloat)height/(GLfloat)width, near_, far_);
  else
    Projection_ = glm::ortho(viewport_left_*(GLfloat)width/(GLfloat)height, viewport_right_ *(GLfloat)width/(GLfloat)height, viewport_bottom_, viewport_top_, near_, far_);  
}

void ViewTransformer::init(int x, int y, int width, int height)
{
  viewport_x_ = x;          viewport_y_ = y;
  viewport_width_ = width;  viewport_height_ = height;
}

void ViewTransformer::Translate(float x, float y, float z)
{
  glm::mat4 T = glm::translate(glm::mat4(1.0f), glm::vec3(x,y,z));
  Model_ = T * Model_; //左乘
}

void ViewTransformer::Rotate(float angle, float x, float y, float z)
{
  glm::mat4 R = glm::rotate(glm::mat4(1.0f),angle,glm::vec3(x, y, z));
  Model_ = R * Model_;  //绕世界坐标系转 左乘
}
void ViewTransformer::RotateR(float angle, float x, float y, float z)
{
  Model_ = glm::rotate(Model_, angle,glm::vec3(x, y, z));
}

void ViewTransformer::Zoom_Out(float ratio)
{
  float width = right_ - left_;
  float height = top_ - bottom_;
  float x_changed = (ratio - float(1)) * width * float(0.5);
  float y_changed = (ratio - float(1)) * height * float(0.5);
  left_ -= x_changed;
  right_ += x_changed;
  bottom_ -= y_changed;
  top_ += y_changed;

  SetProjection(left_, right_, bottom_, top_);
  //std::cout << "x_changed= "  << x_changed << " y_changed= " << y_changed << "\n";
}

void ViewTransformer::CalculateBox()
{
   float width = right_ - left_;
   float height= top_ - bottom_;
   float half_w = width / 2.0f;
   float half_h = height/ 2.0f;

   if (width < height)
   {
     viewport_left_ = (right_ + left_) / 2 - half_h;
     viewport_right_= (right_ + left_) / 2 + half_h;
     viewport_bottom_= bottom_;
     viewport_top_   = top_;
   }
   else if(width > height)
   {
     viewport_bottom_= (top_ + bottom_) /2 - half_w;
     viewport_top_   = (top_ + bottom_) /2 + half_w;
     viewport_left_  = left_;
     viewport_right_ = right_;
   }

  //std::cout << "left= " << viewport_left_ << "  right= " << viewport_right_ << "\nbottom= " << viewport_bottom_ << " top= " << viewport_top_ << "\n";
}

void ViewTransformer::CalculateLod(float resolution)
{
  if (resolution <= 0.03125)
    lod_ = 21;
  else if(resolution > 0.03125 && resolution <= 0.0625)
    lod_ = 20;
  else if(resolution > 0.0625 && resolution <= 0.125)
    lod_ = 19;
  else if(resolution > 0.125 && resolution <= 0.25)
    lod_ = 18;
  else if(resolution > 0.25 && resolution <= 0.5)
    lod_ = 17;
  else if(resolution > 0.5 && resolution <= 1.0f)
    lod_ = 16;
  else if(resolution > 1.0f && resolution <= 2.0f)
    lod_ = 15;
  else if(resolution > 2.0f && resolution <= 4.0f)
    lod_ = 14; 
  else if(resolution > 4.0f && resolution <= 8.0f)
    lod_ = 13;
  else
  {
    lod_ = 12;
  }
}