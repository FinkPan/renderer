#ifndef _HS_GRAPHICS_GRAPHICS_UTILITY_VIEWING_TRANSFORMER_HPP_
#define _HS_GRAPHICS_GRAPHICS_UTILITY_VIEWING_TRANSFORMER_HPP_

#include <bitset>

#include "hs_math/linear_algebra/eigen_macro.hpp"
#include "hs_math/geometry/rotation.hpp"

namespace hs
{
namespace renderer
{

template <typename _Scalar>
class ViewingTransformer
{
public:
  typedef _Scalar Scalar;
  typedef hs::math::geometry::Rotation3D<Scalar> Rotation;
  typedef typename Rotation::OrthoRotMat RotationMatrix;
  typedef typename Rotation::AngleAxis AngleAxis;


  typedef EIGEN_VECTOR(Scalar, 3) Translate;
  typedef EIGEN_MATRIX(Scalar, 4, 4) TransformMatrix;
  typedef EIGEN_MATRIX(Scalar, 3, 3) RMatrix;
  typedef EIGEN_VECTOR(Scalar, 2) Vector2;
  typedef EIGEN_VECTOR(Scalar, 3) Vector3;

  enum ViewingTransformerFlag
  {
    VTF_PERSPECTIVE = 0,
    VTF_KEEP_RATIO,
    NUMBER_OF_VTFS
  };
  typedef std::bitset<NUMBER_OF_VTFS> Mask;

public:
  ViewingTransformer(const Rotation& rotation = Rotation(),
                     const Translate& translate = Translate::Zero(),
                     Scalar scale = 1,
                     Scalar left = Scalar(0),
                     Scalar right = Scalar(0),
                     Scalar top = Scalar(0),
                     Scalar bottom = Scalar(0),
                     Scalar near = Scalar(0),
                     Scalar far = Scalar(0),
                     int viewport_x = 0,
                     int viewport_y = 0,
                     int viewport_width = 0,
                     int viewport_height = 0,
                     const Mask& mask = Mask("01"))
    : rotation_(rotation),
      translate_(translate),
      scale_(scale),
      left_(left),
      right_(right),
      top_(top),
      bottom_(bottom),
      near_(near),
      far_(far),
      viewport_x_(viewport_x),
      viewport_y_(viewport_y),
      viewport_width_(viewport_width),
      viewport_height_(viewport_height),
      mask_(mask)
  {
    mvp_ = TransformMatrix::Identity();
    project_ = TransformMatrix::Identity();
    view_ = TransformMatrix::Identity();
    model_ = TransformMatrix::Identity();
  }

  bool IsPerspective() const
  {
    return mask_[VTF_PERSPECTIVE];
  }

  void SetPerspective(bool is_perspective)
  {
    mask_.set(VTF_PERSPECTIVE, is_perspective);
  }

  bool IsKeepRatio() const
  {
    return mask_[VTF_KEEP_RATIO];
  }

  void SetKeepRatio(bool is_keep_ratio)
  {
    mask_.set(VTF_KEEP_RATIO, is_keep_ratio);
  }

  Scalar GetUintPerPixel()
  {
    return std::abs((vt.right() - vt.left()) /Scalar(vt.viewport_width()));
  }

  TransformMatrix SimilarMatrix() const
  {
    TransformMatrix similar_matrix = TransformMatrix::Identity();
    similar_matrix.block(0, 0, 3, 3) =
      RotationMatrix(rotation_) * scale_;
    similar_matrix.block(0, 3, 3, 1) = translate_;

    return similar_matrix;
  }

  TransformMatrix ProjectionMatrix() const
  {
    TransformMatrix projection_matrix = TransformMatrix::Zero();
    Scalar width = right_ - left_;
    Scalar height = top_ - bottom_;
    Scalar depth = far_ - near_;
    while (1)
    {
      if (width == Scalar(0) || height == Scalar(0) || depth == Scalar(0))
      {
        break;
      }
      Scalar half_w = width / Scalar(2);
      Scalar half_h = height / Scalar(2);
      if (mask_[VTF_PERSPECTIVE])
      {
        projection_matrix <<
          near_ / half_w, Scalar(0), (left_ + right_) / half_w, Scalar(0),
          Scalar(0), near_ / half_h, (top_ + bottom_) / half_h, Scalar(0),
          Scalar(0), Scalar(0), -(far_ + near_) / (depth),
                                         -Scalar(2) * far_ * near_ / depth,
          Scalar(0), Scalar(0), Scalar(-1), Scalar(0);
      }
      else
      {
        projection_matrix <<
          Scalar(1) / half_w, Scalar(0), Scalar(0), -(right_ + left_) / width,
          Scalar(0), Scalar(1) / half_h, Scalar(0), -(top_ + bottom_) / height,
          Scalar(0), Scalar(0), -Scalar(1) / (depth / Scalar(2)),
                                                       -(far_ + near_) / depth,
          Scalar(0), Scalar(0), Scalar(0), Scalar(1);
      }

      break;
    }
    return projection_matrix;
  }

  TransformMatrix ViewingTransformMatrix() const
  {
    //return ProjectionMatrix() * t_ * SimilarMatrix();
    /*TransformMatrixA p;
    p = ProjectionMatrix();*/
    //project_ = ProjectionMatrix();
    //mvp_ = project_ * view_ * model_;

    return ProjectionMatrix() * SimilarMatrix() ;
  }

  void SetViewMatrix(const Vector3& camera_position, 
                     const Vector3& camera_posture)
  {
    translate_ = Translate(camera_position[0], camera_position[1], camera_position[2]);
  }

  void SetProjectionBoundingBox2D(const Vector2& min, const Vector2& max)
  {
    left_ = min[0];
    right_ = max[0];
    bottom_ = min[1];
    top_ = max[1];
    near_ = Scalar(-100000.0);
    far_ = Scalar(100000.0);
    if (IsKeepRatio())
    {
      KeepRatio2D();
    }
  }

  void SetProjectionBoundingBox3D(const Vector3& min, const Vector3& max)
  {
    left_ = min[0];
    right_ = max[0];
    bottom_ = min[1];
    top_ = max[1];
    near_ = Scalar(-100000.0);
    far_ = Scalar(100000.0);

    offset_[0] = (max[0] + min[0])/2; 
    offset_[1] = (max[1] + min[1])/2; 
    offset_[2] = (max[2] + min[2])/2; 



    if (IsKeepRatio())
    {
      KeepRatio2D();
    }

  }

  void Translate2DByView(const Vector2& translate)
  {
    Scalar width = right_ - left_;
    Scalar height = top_ - bottom_;
    Scalar x_ratio = width / Scalar(viewport_width_);
    Scalar y_ratio = height / Scalar(viewport_height_);
    left_ += translate[0] * x_ratio;
    right_ += translate[0] * x_ratio;
    bottom_ += translate[1] * y_ratio;
    top_ += translate[1] * y_ratio;
  }

  void Scale2D(Scalar ratio)
  {
    Scalar width = right_ - left_;
    Scalar height = top_ - bottom_;
    Scalar x_changed = (ratio - Scalar(1)) * width * Scalar(0.5);
    Scalar y_changed = (ratio - Scalar(1)) * height * Scalar(0.5);
    left_ -= x_changed;
    right_ += x_changed;
    bottom_ -= y_changed;
    top_ += y_changed;
  }

  void RotationByView(Scalar angle, Vector3 axis)
  {
    translate_ = -offset_;

     Rotation rotation_matrix = AngleAxis(angle, axis);
     rotation_ = rotation_ * rotation_matrix;
  }

  void RotationByModel(Scalar angle, Vector3 axis)
  {
    TransformMatrix Result = TransformMatrix::Identity();
    Result.block(0, 0, 3, 3) = RotationMatrix(rotation_);
    Result.block(0, 3, 3, 1) = translate_;

    //将物体移动到原点.
    TransformMatrix T = TransformMatrix::Identity();
    T.block(0, 3, 3, 1) = offset_;

    //将物体旋转.
    TransformMatrix R = TransformMatrix::Identity();
    Rotation rotation_matrix = AngleAxis(angle, axis);
    R.block(0, 0, 3, 3) = RotationMatrix(rotation_matrix);

    //将物体移动到原来地方
    TransformMatrix Tm = T.inverse();

    //将物体 移动到原点->旋转->移动回原来地方.
    Result =  Result * T * R * Tm;

    translate_ = Result.block(0, 3, 3, 1);
    rotation_ =  Result.block(0, 0, 3, 3);

  }

  void RotationByWorld(Scalar angle, Vector3 axis)
  {
    TransformMatrix Result = TransformMatrix::Identity();
    Result.block(0, 0, 3, 3) = RotationMatrix(rotation_);
    Result.block(0, 3, 3, 1) = translate_;

    //将物体移动到原点.
    TransformMatrix T = TransformMatrix::Identity();
    T.block(0, 3, 3, 1) = offset_;

    //将物体旋转.
    TransformMatrix R = TransformMatrix::Identity();
    Rotation rotation_matrix = AngleAxis(angle, axis);
    R.block(0, 0, 3, 3) = RotationMatrix(rotation_matrix);

    //将物体移动到原来地方
    TransformMatrix Tm = T.inverse();

    //将物体 移动到原点->旋转->移动回原来地方.
    Result =   T * R * Tm * Result;

    translate_ = Result.block(0, 3, 3, 1);
    rotation_ =  Result.block(0, 0, 3, 3);

  }


  void FocusByViewFrame(const Vector2& view_frame_min,
                        const Vector2& view_frame_max)
  {
     
  }

  void SetViewport(int viewport_x, int viewport_y,
                   int viewport_width, int viewport_height)
  {
    viewport_x_ = viewport_x;
    viewport_y_ = viewport_y;
    viewport_width_ = viewport_width;
    viewport_height_ = viewport_height;

    if (IsKeepRatio())
      KeepRatio2D();
  }

  Scalar& left() 
  {
    return left_;
  }

  Scalar& right() 
  {
    return right_;
  }

  Scalar& bottom() 
  {
    return bottom_;
  }

  Scalar& top() 
  {
    return top_;
  }

  Scalar& n()
  {
    return near_;
  }

  Scalar& f()
  {
    return far_;
  }
  int viewport_x() const
  {
    return viewport_x_;
  }

  int viewport_y() const
  {
    return viewport_y_;
  }

  int viewport_width() const
  {
    return viewport_width_;
  }

  int viewport_height() const
  {
    return viewport_height_;
  }

private:
  void KeepRatio2D()
  {
    if (viewport_width_ == 0 || viewport_height_ == 0) return;
    Scalar width = right_ - left_;
    Scalar height = top_ - bottom_;

    Scalar width_scale = width / Scalar(viewport_width_);
    Scalar height_scale = height / Scalar(viewport_height_);

    Scalar scale = std::max(width_scale, height_scale);

    if (width_scale > height_scale)
    {
      bottom_ -= (viewport_height_ * scale - height) * Scalar(0.5);
      top_ += (viewport_height_ * scale - height) * Scalar(0.5);
    }
    else
    {
      left_ -= (viewport_width_ * scale - width) * Scalar(0.5);
      right_ += (viewport_width_ * scale - width) * Scalar(0.5);
    }
  }

private:
  TransformMatrix mvp_;
  TransformMatrix project_;
  TransformMatrix view_;
  TransformMatrix model_;
  Rotation rotation_;
  Translate translate_;
  Scalar scale_;
  Scalar left_;
  Scalar right_;
  Scalar top_;
  Scalar bottom_;
  Scalar near_;
  Scalar far_;
  Vector3 offset_;
  int viewport_x_;
  int viewport_y_;
  int viewport_width_;
  int viewport_height_;
  Mask mask_;
};

}
}

#endif
