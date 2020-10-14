#include "Transformer.h"

#include <iostream>

Transformer::Transformer()
  : _K(cv::Mat::eye(3, 3, CV_64F))
  , _ext(cv::Mat::eye(3, 4, CV_64F))
  , _model(cv::Mat::eye(4, 4, CV_64F))
{
}

Transformer::Transformer(const cv::Mat& K, const cv::Mat& rvec, const cv::Mat& tvec)
{
}

Transformer::Transformer(const cv::Mat& K, const cv::Mat& rvec, const cv::Mat& tvec, const cv::Mat& model)
{
}

void Transformer::projectPoints(const std::vector<cv::Point3d>& objectPoints,
                                std::vector<cv::Point2d>& imagePoints) const
{
  imagePoints.clear();
  imagePoints.resize(objectPoints.size());

  for (const cv::Point3d& point3d : objectPoints) {
    std::cout << "point3d: " << point3d << std::endl;

    cv::Mat hPoint3d(cv::Mat(4, 1, CV_64F));
    hPoint3d.at<double>(0, 0) = point3d.x;
    hPoint3d.at<double>(1, 0) = point3d.y;
    hPoint3d.at<double>(2, 0) = point3d.z;
    hPoint3d.at<double>(3, 0) = 1.0;

    std::cout << "point3d (homogenous): " << hPoint3d << std::endl;

    cv::Mat hPoint2d(_K * _ext * _model * hPoint3d);

    std::cout << "point2d (homogenous): " << hPoint2d << std::endl;

    cv::Point2d point2d
      ( hPoint2d.at<double>(0, 0) / hPoint2d.at<double>(2, 0)
      , hPoint2d.at<double>(1, 0) / hPoint2d.at<double>(2, 0)
      );

    std::cout << "point2d: " << point2d << std::endl;
    imagePoints.push_back(point2d);
  }
}
