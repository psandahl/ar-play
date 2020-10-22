#include "Transformer.h"

#include <opencv2/calib3d.hpp>

#include <iostream>

Transformer::Transformer()
  : _K(cv::Mat::eye(3, 3, CV_64FC1))
  , _E(cv::Mat::eye(3, 4, CV_64FC1))
  , _M(cv::Mat::eye(4, 4, CV_64FC1))
{
}

Transformer::Transformer(const cv::Mat& K, const cv::Mat& rvec, const cv::Mat& tvec)
  : _K(K)
  , _E(makeE(rvec, tvec))
  , _M(cv::Mat::eye(4, 4, CV_64FC1))
{
}

Transformer::Transformer(const cv::Mat& K, const cv::Mat& rvec, const cv::Mat& tvec, const cv::Mat& model)
  : _K(K)
  , _E(makeE(rvec, tvec))
  , _M(model)
{
}

void Transformer::projectPoints(const std::vector<cv::Point3d>& objectPoints,
                                std::vector<cv::Point2d>& imagePoints) const
{
  imagePoints.clear();

  // Pre multiply all matrices.
  cv::Mat matrix(_K * _E * _M);

  for (const cv::Point3d& point3d : objectPoints) {
    // Create a homogenous coordinate of the object point.
    cv::Mat point3dH(toHom3d(point3d));

    // Multiply and normalize back to euclidian image space.
    cv::Point2d point2d(toEucl2d(matrix * point3dH));

    imagePoints.push_back(point2d);
  }
}

cv::Point3d Transformer::modelMatrixTransform(const cv::Point3d& point) const
{
  return toEucl3d(_M * toHom3d(point));
}

cv::Point3d Transformer::centerOfProjection() const
{
  cv::Mat m = rot().t() * trans().mul(-1.0);

  return { m.at<double>(0), m.at<double>(1), m.at<double>(2) };
}

cv::Mat Transformer::makeE(const cv::Mat& rvec, const cv::Mat& tvec) const
{
  cv::Mat rmat;
  cv::Rodrigues(rvec, rmat);

  cv::Mat E(3, 4, CV_64FC1);
  E.at<double>(0, 0) = rmat.at<double>(0, 0);
  E.at<double>(1, 0) = rmat.at<double>(1, 0);
  E.at<double>(2, 0) = rmat.at<double>(2, 0);

  E.at<double>(0, 1) = rmat.at<double>(0, 1);
  E.at<double>(1, 1) = rmat.at<double>(1, 1);
  E.at<double>(2, 1) = rmat.at<double>(2, 1);

  E.at<double>(0, 2) = rmat.at<double>(0, 2);
  E.at<double>(1, 2) = rmat.at<double>(1, 2);
  E.at<double>(2, 2) = rmat.at<double>(2, 2);

  E.at<double>(0, 3) = tvec.at<double>(0);
  E.at<double>(1, 3) = tvec.at<double>(1);
  E.at<double>(2, 3) = tvec.at<double>(2);

  return E;
}

cv::Mat Transformer::rot() const
{
  cv::Mat r(3, 3, CV_64FC1);

  r.at<double>(0, 0) = _E.at<double>(0, 0);
  r.at<double>(1, 0) = _E.at<double>(1, 0);
  r.at<double>(2, 0) = _E.at<double>(2, 0);

  r.at<double>(0, 1) = _E.at<double>(0, 1);
  r.at<double>(1, 1) = _E.at<double>(1, 1);
  r.at<double>(2, 1) = _E.at<double>(2, 1);

  r.at<double>(0, 2) = _E.at<double>(0, 2);
  r.at<double>(1, 2) = _E.at<double>(1, 2);
  r.at<double>(2, 2) = _E.at<double>(2, 2);

  return r;
}

cv::Mat Transformer::trans() const
{
  cv::Mat t(3, 1, CV_64FC1);

  t.at<double>(0) = _E.at<double>(0, 3);
  t.at<double>(1) = _E.at<double>(1, 3);
  t.at<double>(2) = _E.at<double>(2, 3);

  return t;
}

cv::Mat Transformer::toHom3d(const cv::Point3d& point) const
{
  cv::Mat vec(4, 1, CV_64FC1);

  vec.at<double>(0) = point.x;
  vec.at<double>(1) = point.y;
  vec.at<double>(2) = point.z;
  vec.at<double>(3) = 1.0;

  return vec;
}

cv::Point2d Transformer::toEucl2d(const cv::Mat& vec) const
{
  return
    { vec.at<double>(0) / vec.at<double>(2)
    , vec.at<double>(1) / vec.at<double>(2)
    };
}

cv::Point3d Transformer::toEucl3d(const cv::Mat& vec) const
{
  return
    { vec.at<double>(0) / vec.at<double>(3)
    , vec.at<double>(1) / vec.at<double>(3)
    , vec.at<double>(2) / vec.at<double>(3)
    };
}
