#pragma once

#include <opencv2/core.hpp>

#include <vector>

class Transformer;

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

  // Get the face texture.
  const cv::Mat& faceTexture() const { return _faceTexture; }

  // Get the model matrix.
  const cv::Mat& modelMatrix() const { return _modelMatrix; }

  // Set the transform for the model.
  void setTransform(double x, double y, double z);

  // Perform debug rendering of the model.
  void renderDebug(cv::Mat& image, const Transformer& t) const;

private:

  struct Face {
    Face()
      : p0(0), p1(0), p2(0), p3(0)
    {}

    Face(int _p0, int _p1, int _p2, int _p3)
      : p0(_p0), p1(_p1), p2(_p2), p3(_p3)
    {}

    int p0;
    int p1;
    int p2;
    int p3;
  };

  // Render a textured face.
  void renderFace(cv::Mat& image, const std::vector<cv::Point2d>& dstPoints) const;

  // Board dimension (should be power of two, or at least even).
  static constexpr int Size = 128;

  // Chessboard texture for a face.
  cv::Mat _faceTexture;

  // Model matrix for the cube.
  cv::Mat _modelMatrix;

  // Model cube corners.
  std::vector<cv::Point3d> _cornerPoints;

  // Cube faces.
  std::vector<Face> _faces;
};
