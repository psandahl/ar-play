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
  , _M(cv::Mat::eye(4, 4, CV_64FC1))
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
    cv::Mat point3dH(4, 1, CV_64FC1);
    point3dH.at<double>(0) = point3d.x;
    point3dH.at<double>(1) = point3d.y;
    point3dH.at<double>(2) = point3d.z;
    point3dH.at<double>(3) = 1.0;

    // Multiply the object point to get a homogenous image point.
    cv::Mat point2dH(3, 1, CV_64FC1);
    point2dH = matrix * point3dH;

    // Normalize to euclidian image space.
    cv::Point2d point2d
      ( point2dH.at<double>(0) / point2dH.at<double>(2)
      , point2dH.at<double>(1) / point2dH.at<double>(2)
      );

    imagePoints.push_back(point2d);
  }
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
