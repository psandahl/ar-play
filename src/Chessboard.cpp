#include "Chessboard.h"

#include <opencv2/calib3d.hpp>

#include <cassert>

Chessboard::Chessboard(const cv::Size& size, float meters)
  : _size(size)
  , _meters(meters)
{
}

bool Chessboard::detect(const cv::Mat& image, std::vector<cv::Point2f>& points) const
{
  points.clear();
  points.resize(_size.width * _size.height);

  return cv::findChessboardCorners
    ( image, _size, points
    , cv::CALIB_CB_ADAPTIVE_THRESH |
      cv::CALIB_CB_NORMALIZE_IMAGE |
      cv::CALIB_CB_FAST_CHECK
    );
}

std::vector<cv::Point2i> Chessboard::cornerPointsi(const std::vector<cv::Point2f>& points) const
{
  assert(points.size() == _size.width * _size.height);

  return
    { points[0]
    , points[_size.width - 1]
    , points[_size.height * _size.width - 1]
    , points[(_size.height - 1) * _size.width]
    };
}

std::vector<cv::Point2d> Chessboard::cornerPointsd(const std::vector<cv::Point2f>& points) const
{
  assert(points.size() == _size.width * _size.height);

  return
    { points[0]
    , points[_size.width - 1]
    , points[_size.height * _size.width - 1]
    , points[(_size.height - 1) * _size.width]
    };
}
