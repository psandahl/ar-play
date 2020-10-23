#pragma once

#include "ChessboardCalib.h"
#include "ChessboardDetect.h"
#include "ChessboardModel.h"
#include "Transformer.h"

#include <opencv2/core.hpp>

#include <vector>

class Augmenter {
public:
  // Construction.
  Augmenter(cv::Mat coveringImage = cv::Mat());

  // Destruction.
  ~Augmenter() = default;

  // Disabled standard methods.
  Augmenter(const Augmenter&) = delete;
  Augmenter(Augmenter&&) = delete;
  Augmenter& operator=(const Augmenter&) = delete;
  Augmenter& operator=(Augmenter&&) = delete;

  // Process the given image.
  cv::Mat process(const cv::Mat& frame);

private:

  // Run calibration for the image.
  void runCalibration(cv::Mat& image, const std::vector<cv::Point2f>& chessboardPoints);

  // Run augmentation for the image.
  void runAugmentation(cv::Mat& image, const std::vector<cv::Point2f>& chessboardPoints);

  // Draw orientation axes.
  void drawOrientationAxes(cv::Mat& image, const Transformer& t) const;

  // Draw the chessboard covering image.
  void drawCoveringImage(cv::Mat& image, const std::vector<cv::Point2d>& chessboardCorners) const;

  // The size of the board - width and height.
  const cv::Size _boardSize;

  // The chessboard calibrator.
  ChessboardCalib _chessboardCalib;

  // The chessboard detector.
  ChessboardDetect _chessboardDetect;

  // The chessboard model.
  ChessboardModel _chessboardModel;

  // Model points representing ground zero, and used for pose calculation
  // from the chessboard.
  std::vector<cv::Point3d> _groundPoints;

  // A covering image for the detected chessboard.
  const cv::Mat _coveringImage;
};
