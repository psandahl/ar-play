#include "Augmenter.h"
#include "Transformer.h"
#include "Util.h"

#include <opencv2/calib3d.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>
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
  std::vector<cv::Point2i> cornerPoints = points2i(_chessboardDetect.cornerPoints(chessboardPoints));
  cv::polylines(image, cornerPoints, true, cv::Scalar(0, 0, 255));
  _chessboardCalib.addImagePoints(chessboardPoints, image.size());
}

void Augmenter::runAugmentation(cv::Mat& image, const std::vector<cv::Point2f>& chessboardPoints)
{
  // Get the corner points for the detected boad.
  const std::vector<cv::Point2d> boardCornerPoints
    (
      points2d(_chessboardDetect.cornerPoints(chessboardPoints))
    );

  // Get a mapping to 3D-space.
  cv::Mat rvec, tvec;
  if (cv::solvePnP(_groundPoints, boardCornerPoints, _chessboardCalib.getK()
                  , _chessboardCalib.getDistCoeffs(), rvec, tvec)) {

    /*std::vector<cv::Point2f> imagePointsf;
    cv::Mat zd(cv::Mat::zeros(5, 1, CV_64FC1));
    cv::projectPoints(points3f(_groundPoints), rvec, tvec, _chessboardCalib.getK(), zd, imagePointsf);
    std::cout << "p> " << imagePointsf[0] << std::endl;*/

    const Transformer t(_chessboardCalib.getK(), rvec, tvec);

    std::vector<cv::Point2d> imageCornerPoints;
    t.projectPoints(_groundPoints, imageCornerPoints);


    cv::polylines(image, points2i(imageCornerPoints), true, cv::Scalar(0, 255, 0));
  }
}
