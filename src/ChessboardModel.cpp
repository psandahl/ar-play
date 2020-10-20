#include "ChessboardModel.h"

#include <opencv2/calib3d.hpp>
#include <opencv2/imgproc.hpp>

ChessboardModel::ChessboardModel()
  : _faceTexture(Size, Size, CV_8UC3, cv::Scalar::all(255))
{
  cv::rectangle(_faceTexture, { 0, 0 }, { Size / 2 - 1, Size / 2 - 1 },
                cv::Scalar::all(0), cv::FILLED);
  cv::rectangle(_faceTexture, { Size / 2, Size / 2 }, { Size - 1, Size - 1 },
                cv::Scalar::all(0), cv::FILLED);
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
