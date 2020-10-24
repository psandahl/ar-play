#include "Augmenter.h"
#include "Util.h"

#include <opencv2/calib3d.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>
#include <vector>

Augmenter::Augmenter(cv::Mat coveringImage)
  : _boardSize(9, 6)
  , _chessboardCalib(_boardSize, 0.025)
  , _chessboardDetect(_boardSize)
  , _chessboardModel()
  , _groundPoints(4)
  , _coveringImage(coveringImage)
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
  //std::vector<cv::Point2i> cornerPoints = points2i(_chessboardDetect.cornerPoints(chessboardPoints));
  //cv::polylines(image, cornerPoints, true, cv::Scalar(0, 0, 255));
  _chessboardCalib.addImagePoints(chessboardPoints, image.size());
}

void Augmenter::runAugmentation(cv::Mat& image, const std::vector<cv::Point2f>& chessboardPoints)
{
  // Get the corner points for the detected chessboard.
  const std::vector<cv::Point2d> boardCornerPoints
    (
      points2d(_chessboardDetect.cornerPoints(chessboardPoints))
    );

  // Get a mapping to 3D-space.
  cv::Mat rvec, tvec;
  if (cv::solvePnP(_groundPoints, boardCornerPoints, _chessboardCalib.getK()
                  , _chessboardCalib.getDistCoeffs(), rvec, tvec)) {

    // Draw the covering image.
    drawCoveringImage(image, boardCornerPoints);

    // Animate the model.
    _chessboardModel.animate();

    // A mapping from the chessboard marker and the ground points is found.
    // Create a transformer from model coordinate to image coordinates.
    const Transformer t(_chessboardCalib.getK(), rvec, tvec, _chessboardModel.modelMatrix());

    // Render the model.
    _chessboardModel.render(image, t);

    // Draw the orientation axes.
    //drawOrientationAxes(image, t);
  }
}

void Augmenter::drawOrientationAxes(cv::Mat& image, const Transformer& t) const
{
  std::vector<cv::Point3d> modelSpaceAxes =
    { _groundPoints[0]
    , _groundPoints[0] + cv::Point3d(1.0, 0.0, 0.0)
    , _groundPoints[0] + cv::Point3d(0.0, 1.0, 0.0)
    , _groundPoints[0] + cv::Point3d(0.0, 0.0, 1.0)
    };

  std::vector<cv::Point2d> imageSpaceAxes;
  t.projectPoints(modelSpaceAxes, imageSpaceAxes);

  std::vector<cv::Point2i> imageSpaceAxesi = points2i(imageSpaceAxes);
  cv::line(image, imageSpaceAxesi[0], imageSpaceAxesi[1], cv::Scalar(0, 0, 255), 3);
  cv::line(image, imageSpaceAxesi[0], imageSpaceAxesi[2], cv::Scalar(0, 255, 0), 3);
  cv::line(image, imageSpaceAxesi[0], imageSpaceAxesi[3], cv::Scalar(255, 0, 0), 3);
}

void Augmenter::drawCoveringImage(cv::Mat& image, const std::vector<cv::Point2d>& chessboardCorners) const
{
  if (!_coveringImage.empty()) {
    const double width = double(_coveringImage.size().width);
    const double height = double(_coveringImage.size().height);
    const double xmargin = width * 0.2;
    const double ymargin = height * 0.22;

    const std::vector<cv::Point2d> imageCorners =
      { { xmargin              , ymargin }
      , { width - xmargin - 1.0, ymargin }
      , { width - xmargin - 1.0, height - ymargin - 1.0 }
      , { xmargin              , height - ymargin - 1.0 }
      };

    const cv::Mat H = cv::findHomography(imageCorners, chessboardCorners);
    if (!H.empty()) {
      cv::warpPerspective(_coveringImage, image, H, image.size(),
                          cv::INTER_LINEAR, cv::BORDER_TRANSPARENT);
    }
  }
}
