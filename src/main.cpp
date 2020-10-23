#include "Augmenter.h"

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>

#include <iostream>
#include <string>

int main(int argc, char** argv)
{
  const char* params =
    "{ help h  | | print help information }"
    "{ image i | | path to the covering image }"
    "{ video v | | path to the video to be played }";

  cv::CommandLineParser parser(argc, argv, params);
  if (!parser.check()) {
    parser.printErrors();
    return -1;
  }

  if (parser.has("help") || !parser.has("image") || !parser.has("video")) {
    parser.about("AR playground");
    parser.printMessage();
    return -1;
  }

  const std::string imagePath(parser.get<std::string>("image"));
  cv::Mat coveringImage(cv::imread(imagePath, cv::IMREAD_COLOR));
  if (coveringImage.empty()) {
    std::cerr << "Failed to open image '" << imagePath
              << "'" << std::endl;
    return -1;
  }

  const std::string videoPath(parser.get<std::string>("video"));
  cv::VideoCapture capture(videoPath);
  if (!capture.isOpened()) {
    std::cerr << "Failed to open video '" << videoPath
              << "'" << std::endl;
    return -1;
  }

  const std::string window("Augmented Reality Play");
  cv::namedWindow(window);

  Augmenter augmenter(coveringImage);
  cv::Mat frame;
  while (capture.read(frame)) {
    cv::imshow(window, augmenter.process(frame));
    cv::waitKey(1);
  }

  cv::destroyAllWindows();

  return 0;
}
