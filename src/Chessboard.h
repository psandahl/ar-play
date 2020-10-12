#pragma once

#include <opencv2/core.hpp>

#include <vector>

class Chessboard {
public:
  // Construction.
  Chessboard(const cv::Size& size, float meters);

  // Destruction.
  ~Chessboard() = default;

  // Disabled standard methods.
  Chessboard() = delete;
  Chessboard(const Chessboard&) = delete;
  Chessboard(Chessboard&&) = delete;

  Chessboard& operator=(const Chessboard&) = delete;
  Chessboard& operator=(Chessboard&&) = delete;

  // Check an image for a chessboard pattern.
  bool detect(const cv::Mat& image, std::vector<cv::Point2f>& points) const;

  // From a detected chessboard pattern extract the corners points for the quad.
  std::vector<cv::Point2i> cornerPointsi(const std::vector<cv::Point2f>& points) const;
  std::vector<cv::Point2d> cornerPointsd(const std::vector<cv::Point2f>& points) const;

private:

  const cv::Size _size;
  const float _meters;
};
