#pragma once

#include "ChessboardCalib.h"
#include "ChessboardDetect.h"

#include <opencv2/core.hpp>

#include <vector>

class Augmenter {
public:
  // Construction.
  Augmenter();

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

  // The size of the board - width and height.
  const cv::Size _boardSize;

  // The chessboard calibrator.
  ChessboardCalib _chessboardCalib;

  // The chessboard detector.
  ChessboardDetect _chessboardDetect;

  // Model points representing ground zero, and used for pose calculation
  // from the chessboard.
  std::vector<cv::Point3d> _groundPoints;
};
