#pragma once

#include "ChessboardDetect.h"

#include <opencv2/core.hpp>

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

  // The chessboard detector.
  ChessboardDetect _chessboardDetect;
};
