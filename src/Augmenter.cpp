#include "Augmenter.h"

#include <opencv2/imgproc.hpp>

#include <vector>

Augmenter::Augmenter()
  : _chessboard({9, 6}, 0.025)
{
}

cv::Mat Augmenter::process(const cv::Mat& frame)
{
  cv::Mat image(frame.clone());

  std::vector<cv::Point2f> chessboardPoints;
  if (_chessboard.detect(image, chessboardPoints)) {
    std::vector<cv::Point2i> cornerPoints = _chessboard.cornerPointsi(chessboardPoints);
    cv::polylines(image, cornerPoints, true, cv::Scalar(0, 255, 0));
  }

  return image;
}
