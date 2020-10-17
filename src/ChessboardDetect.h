#pragma once

#include <opencv2/core.hpp>

#include <vector>

class ChessboardDetect {
public:
  // Construction.
  ChessboardDetect(const cv::Size& size);

  // Destruction.
  ~ChessboardDetect() = default;

  // Disabled standard methods.
  ChessboardDetect() = delete;
  ChessboardDetect(const ChessboardDetect&) = delete;
  ChessboardDetect(ChessboardDetect&&) = delete;

  ChessboardDetect& operator=(const ChessboardDetect&) = delete;
  ChessboardDetect& operator=(ChessboardDetect&&) = delete;

  // Check an image for a chessboard pattern.
  bool detect(const cv::Mat& image, std::vector<cv::Point2f>& points) const;

  // From a detected chessboard pattern extract the corners points for the quad.
  std::vector<cv::Point2f> cornerPoints(const std::vector<cv::Point2f>& points) const;

private:

  // Dimensions of the board.
  const cv::Size _size;
};
