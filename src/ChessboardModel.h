#pragma once

#include <opencv2/core.hpp>

#include <vector>

class ChessboardModel {
public:
  // Construction.
  ChessboardModel();

  // Destruction.
  ~ChessboardModel() = default;

  // Disabled standard methods.
  ChessboardModel(const ChessboardModel&) = delete;
  ChessboardModel(ChessboardModel&&) = delete;

  ChessboardModel& operator=(const ChessboardModel&) = delete;
  ChessboardModel& operator=(ChessboardModel&&) = delete;

  // Get the texture.
  const cv::Mat& faceTexture() const { return _faceTexture; }

  void renderFace(cv::Mat& image, const std::vector<cv::Point2d>& dstPoints) const;

private:
  // Board dimension (should be power of two, or at least even).
  static constexpr int Size = 128;

  // Chessboard texture for one face.
  cv::Mat _faceTexture;
};
