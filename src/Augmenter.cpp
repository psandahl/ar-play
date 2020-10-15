#include "Augmenter.h"

#include <opencv2/imgproc.hpp>

#include <vector>

Augmenter::Augmenter()
  : _boardSize(9, 6)
  , _chessboardCalib(_boardSize, 0.025)
  , _chessboardDetect(_boardSize)
  , _groundPoints(4)
{
  const double halfWidth = double(_boardSize.width) * 0.5;
  const double halfHeight = double(_boardSize.height) * 0.5;

  // Points counter clockwise.
  _groundPoints[0] = cv::Point3d(-halfWidth, -halfHeight, 0.0);
  _groundPoints[1] = cv::Point3d( halfWidth, -halfHeight, 0.0);
  _groundPoints[2] = cv::Point3d( halfWidth,  halfHeight, 0.0);
  _groundPoints[3] = cv::Point3d(-halfWidth,  halfHeight, 0.0);
}

cv::Mat Augmenter::process(const cv::Mat& frame)
{
  cv::Mat image(frame.clone());

  std::vector<cv::Point2f> chessboardPoints;
  if (_chessboardDetect.detect(image, chessboardPoints)) {
    if (_chessboardCalib.isCalibrated()) {
      runAugmentation(image, chessboardPoints);
    } else {
      runCalibration(image, chessboardPoints);
    }
  }

  return image;
}

void Augmenter::runCalibration(cv::Mat& image, const std::vector<cv::Point2f>& chessboardPoints)
{
  std::vector<cv::Point2i> cornerPoints = _chessboardDetect.cornerPointsi(chessboardPoints);
  cv::polylines(image, cornerPoints, true, cv::Scalar(0, 0, 255));
  _chessboardCalib.addImagePoints(chessboardPoints, image.size());
}

void Augmenter::runAugmentation(cv::Mat& image, const std::vector<cv::Point2f>& chessboardPoints)
{
  std::vector<cv::Point2i> cornerPoints = _chessboardDetect.cornerPointsi(chessboardPoints);
  cv::polylines(image, cornerPoints, true, cv::Scalar(0, 255, 0));
}
