#include "ChessboardDetect.h"

#include <opencv2/calib3d.hpp>

#include <cassert>

ChessboardDetect::ChessboardDetect(const cv::Size& size)
  : _size(size)
{
}

bool ChessboardDetect::detect(const cv::Mat& image, std::vector<cv::Point2f>& points) const
{
  points.clear();

  return cv::findChessboardCorners
    ( image, _size, points
    , cv::CALIB_CB_ADAPTIVE_THRESH |
      cv::CALIB_CB_NORMALIZE_IMAGE |
      cv::CALIB_CB_FAST_CHECK
    );
}

std::vector<cv::Point2f> ChessboardDetect::cornerPoints(const std::vector<cv::Point2f>& points) const
{
  assert(points.size() == _size.width * _size.height);

  return
    { points[0]
    , points[_size.width - 1]
    , points[_size.height * _size.width - 1]
    , points[(_size.height - 1) * _size.width]
    };
}
