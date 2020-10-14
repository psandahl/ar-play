#include "Augmenter.h"

#include <opencv2/imgproc.hpp>

#include <vector>

Augmenter::Augmenter()
  : _chessboardCalib({ 9, 6 }, 0.025)
  , _chessboardDetect({ 9, 6 })
{
}

cv::Mat Augmenter::process(const cv::Mat& frame)
{
  cv::Mat image(frame.clone());

  std::vector<cv::Point2f> chessboardPoints;
  if (_chessboardDetect.detect(image, chessboardPoints)) {
    std::vector<cv::Point2i> cornerPoints = _chessboardDetect.cornerPointsi(chessboardPoints);

    if (_chessboardCalib.isCalibrated()) {
      cv::polylines(image, cornerPoints, true, cv::Scalar(0, 255, 0));
    } else {
      cv::polylines(image, cornerPoints, true, cv::Scalar(0, 0, 255));
      _chessboardCalib.addImagePoints(chessboardPoints, image.size());
    }    
  }

  return image;
}
