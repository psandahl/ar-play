#pragma once

#include <opencv2/core.hpp>

#include <vector>

class Transformer {
public:
  // Construction.
  Transformer();
  Transformer(const cv::Mat& K, const cv::Mat& rvec, const cv::Mat& tvec);
  Transformer(const cv::Mat& K, const cv::Mat& rvec, const cv::Mat& tvec, const cv::Mat& model);

  Transformer(const Transformer&) = default;
  Transformer& operator=(const Transformer&) = default;

  // Destruction.
  ~Transformer() = default;

  // Project a set of object points to image points.
  void projectPoints(const std::vector<cv::Point3d>& objectPoints, std::vector<cv::Point2d>& imagePoints) const;

private:

  cv::Mat makeE(const cv::Mat& rvec, const cv::Mat& tvec) const;

  cv::Mat _K;
  cv::Mat _E;
  cv::Mat _M;
};
