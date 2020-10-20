#pragma once

#include <opencv2/core.hpp>

#include <vector>

std::vector<cv::Point2i> points2i(const std::vector<cv::Point2f>& points);
std::vector<cv::Point2i> points2i(const std::vector<cv::Point2d>& points);
std::vector<cv::Point2d> points2d(const std::vector<cv::Point2f>& points);
std::vector<cv::Point2f> points2f(const std::vector<cv::Point2d>& points);

std::vector<cv::Point3f> points3f(const std::vector<cv::Point3d>& points);
