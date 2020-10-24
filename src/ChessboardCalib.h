#pragma once

#include <opencv2/core.hpp>

#include <vector>

class ChessboardCalib {
public:
  // Construction.
  ChessboardCalib(const cv::Size& size, float meters);

  // Destruction.
  ~ChessboardCalib() = default;

  // Disabled standard methods.
  ChessboardCalib() = delete;
  ChessboardCalib(const ChessboardCalib&) = delete;
  ChessboardCalib(ChessboardCalib&&) = delete;

  ChessboardCalib& operator=(const ChessboardCalib&) = delete;
  ChessboardCalib& operator=(ChessboardCalib&&) = delete;

  // Get the instrinsics matrix.
  const cv::Mat& getK() const { return _K; }

  // Get the distortion coefficients.
  const cv::Mat& getDistCoeffs() const { return _distCoeffs; }

  // Tell if the calibration is done or not.
  bool isCalibrated() const { return _isCalibrated; }

  // Add image points to the calibration. Eventually the calibration will be
  // complete.
  void addImagePoints(const std::vector<cv::Point2f>& imagePoints, const cv::Size& imageSize);

private:
  typedef std::vector<cv::Point2f> ImagePoints;
  typedef std::vector<cv::Point3f> ObjectPoints;

  void generateChessboard(ObjectPoints& objectPoints) const;

  // Dimensions of the board.
  const cv::Size _size;

  // Size between the corners, expressed in meters.
  const float _meters;

  // K matrix.
  cv::Mat _K;

  // Distortion coefficients.
  cv::Mat _distCoeffs;

  // The collection of image points.
  std::vector<ImagePoints> _imagePointsCollection;

  // Needed points for calibration.
  static constexpr int _neededPoints = 25;

  // Nth frame counter.
  int _nthFrame;

  bool _isCalibrated;
};
