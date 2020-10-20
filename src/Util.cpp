#include "Util.h"

#include <cmath>

std::vector<cv::Point2i> points2i(const std::vector<cv::Point2f>& points)
{
  std::vector<cv::Point2i> points2i;

  for (const cv::Point2f& point : points) {
    points2i.push_back({ int(std::round(point.x)), int(std::round(point.y)) });
  }

  return points2i;
}

std::vector<cv::Point2i> points2i(const std::vector<cv::Point2d>& points)
{
  std::vector<cv::Point2i> points2i;

  for (const cv::Point2f& point : points) {
    points2i.push_back({ int(std::round(point.x)), int(std::round(point.y)) });
  }

  return points2i;
}

std::vector<cv::Point2d> points2d(const std::vector<cv::Point2f>& points)
{
  std::vector<cv::Point2d> points2d;

  for (const cv::Point2f& point : points) {
    points2d.push_back(point);
  }

  return points2d;
}

std::vector<cv::Point2f> points2d(const std::vector<cv::Point2d>& points)
{
  std::vector<cv::Point2f> points2f;

  for (const cv::Point2d& point : points) {
    points2f.push_back({ float(point.x), float(point.y) });
  }

  return points2f;
}

std::vector<cv::Point3f> points3f(const std::vector<cv::Point3d>& points)
{
  std::vector<cv::Point3f> points3f;

  for (const cv::Point3d& point : points) {
    points3f.push_back({ float(point.x), float(point.y), float(point.z) });
  }

  return points3f;
}
