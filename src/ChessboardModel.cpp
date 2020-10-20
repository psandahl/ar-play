#include "ChessboardModel.h"
#include "Transformer.h"
#include "Util.h"

#include <opencv2/calib3d.hpp>
#include <opencv2/imgproc.hpp>

ChessboardModel::ChessboardModel()
  : _faceTexture(Size, Size, CV_8UC3, cv::Scalar::all(255))
  , _modelMatrix(cv::Mat::eye(4, 4, CV_64FC1))
  , _cornerPoints(8)
{
  // Render the face texture.
  cv::rectangle(_faceTexture, { 0, 0 }, { Size / 2 - 1, Size / 2 - 1 },
                cv::Scalar::all(0), cv::FILLED);
  cv::rectangle(_faceTexture, { Size / 2, Size / 2 }, { Size - 1, Size - 1 },
                cv::Scalar::all(0), cv::FILLED);

  // Model space corner points (height is negative z).
  // Top:
  _cornerPoints[0] = { -1.0, -1.0, -1.0 };
  _cornerPoints[1] = {  1.0, -1.0, -1.0 };
  _cornerPoints[2] = {  1.0,  1.0, -1.0 };
  _cornerPoints[3] = { -1.0,  1.0, -1.0 };
  // Bottom:
  _cornerPoints[4] = { -1.0, -1.0,  1.0 };
  _cornerPoints[5] = {  1.0, -1.0,  1.0 };
  _cornerPoints[6] = {  1.0,  1.0,  1.0 };
  _cornerPoints[7] = { -1.0,  1.0,  1.0 };

  //setTransform(100.0, 0.0, 0.0);
}

void ChessboardModel::setTransform(double x, double y, double z)
{
  _modelMatrix.at<double>(3, 0) = x;
  _modelMatrix.at<double>(3, 1) = y;
  _modelMatrix.at<double>(3, 2) = z;
}

void ChessboardModel::renderDebug(cv::Mat& image, const Transformer& t) const
{
  std::vector<cv::Point2d> imagePointsd;
  t.projectPoints(_cornerPoints, imagePointsd);

  std::vector<cv::Point2i> imagePointsi = points2i(imagePointsd);
  int n = 0;
  for (const cv::Point2i& p : imagePointsi) {
    cv::Scalar col = n++ < 4 ? cv::Scalar(0, 255, 0) : cv::Scalar(0, 0, 255);
    cv::circle(image, p, 2, col, cv::FILLED);
  }
}

void ChessboardModel::renderFace(cv::Mat& image, const std::vector<cv::Point2d>& dstPoints) const
{
  std::vector<cv::Point2f> srcPoints =
    { { 0.0,        0.0 }
    , { Size - 1.0, 0.0}
    , { Size - 1.0, Size - 1.0}
    , { 0.0,        Size - 1.0 }
    };
  cv::Mat H = cv::findHomography(srcPoints, dstPoints);
  if (!H.empty()) {
    cv::warpPerspective(_faceTexture, image, H, image.size(),
                        cv::INTER_LINEAR, cv::BORDER_TRANSPARENT);
  }
}
