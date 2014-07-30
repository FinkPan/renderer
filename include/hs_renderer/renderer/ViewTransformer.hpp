#ifndef _HS_RENDER_OBJRENDERER_VIEWTRANSFORMER_HPP_
#define _HS_RENDER_OBJRENDERER_VIEWTRANSFORMER_HPP_

#include "GL/glew.h"
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr

class ViewTransformer
{
public:
  friend class CubeRenderer;

  enum ProjectionMode
  {
    PERSPECTIVE  = 0,
    ORTHOGRAPHIC
  };


  ViewTransformer();
  ~ViewTransformer();

  const float* GetMVP();
  const float* GetPV();
  const glm::mat4 GetModel() const;

  void SetProjection(float left, float right, float bottom, float top, ProjectionMode projectionMode = ORTHOGRAPHIC);

  void SetViewport(int x, int y, int width, int height);
  void Update();
  void init(int x, int y, int width, int height);

  inline float left()  const { return left_; }
  inline float right() const { return right_; }
  inline float top()   const { return top_; }
  inline float bottom()const { return bottom_; }
  inline float n()   const { return near_; }
  inline float f()const { return far_; }
  inline int lod() const { return lod_; }
  inline int viewport_width() const { return viewport_width_; }
  inline int viewport_height() const { return viewport_height_; }
  inline float vleft() const { return viewport_left_;}
  inline float vright() const { return viewport_right_; }
  inline float vbottom() const { return viewport_bottom_; }
  inline float vtop() const { return viewport_top_; }

  void Translate(float x, float y, float z);
  void Rotate(float angle, float x, float y, float z);
  void RotateR(float angle, float x, float y, float z);
  void Zoom_Out(float ratio);


private:
  void CalculateBox();
  void CalculateLod(float resolution);

  glm::mat4 Projection_;
  glm::mat4 View_;
  glm::mat4 Model_;
  glm::mat4 MVP_;
  glm::mat4 axis_MVP_;

  float left_;
  float right_;
  float top_;
  float bottom_;
  float near_;
  float far_;

  float viewport_left_;
  float viewport_right_;
  float viewport_top_;
  float viewport_bottom_;


  int viewport_x_;
  int viewport_y_;
  int viewport_width_;
  int viewport_height_;

  int lod_;

};

#endif