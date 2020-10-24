#include "ChessboardCalib.h"

#include <opencv2/calib3d.hpp>

ChessboardCalib::ChessboardCalib(const cv::Size& size, float meters)
  : _size(size)
  , _meters(meters)
  , _K(cv::Mat::eye(3, 3, CV_64FC1))
  , _distCoeffs(cv::Mat::zeros(5, 1, CV_64FC1))
  , _imagePointsCollection()
  , _nthFrame(0)
  , _isCalibrated(false)
{
}

void ChessboardCalib::addImagePoints(const std::vector<cv::Point2f>& imagePoints, const cv::Size& imageSize)
{
  if (!isCalibrated()) {
    if (++_nthFrame < 5) return;

    _nthFrame = 0;

    // TODO: Check that the camera poses are different.
    _imagePointsCollection.push_back(imagePoints);

    if (_imagePointsCollection.size() == _neededPoints) {
      ObjectPoints objectPoints;
      generateChessboard(objectPoints);

      std::vector<ObjectPoints> objectPointsCollection(_imagePointsCollection.size(), objectPoints);
      std::vector<cv::Mat> rvecs, tvecs;
      cv::calibrateCamera
        ( objectPointsCollection, _imagePointsCollection
        , imageSize, _K, _distCoeffs
        , rvecs, tvecs
        );

      _isCalibrated = true;
      _imagePointsCollection.clear();
    }
  }
}

void ChessboardCalib::generateChessboard(ObjectPoints& objectPoints) const
{
  for (int y = 0; y < _size.height; ++y) {
    for (int x = 0; x < _size.width; ++x) {
      objectPoints.push_back
        (
          { float(x) * _meters, float(y) * _meters, 0.0 }
        );
    }
  }
}
