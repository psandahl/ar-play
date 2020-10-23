#include "ChessboardModel.h"
#include "Transformer.h"
#include "Util.h"

#include <opencv2/calib3d.hpp>
#include <opencv2/imgproc.hpp>

ChessboardModel::ChessboardModel()
  : _faceTexture(Size, Size, CV_8UC3, cv::Scalar::all(255))
  , _modelMatrix(cv::Mat::eye(4, 4, CV_64FC1))
  , _cornerPoints(8)
  , _faces(6)
{
  // Render the face texture.
  cv::rectangle(_faceTexture, { 0, 0 }, { Size / 2 - 1, Size / 2 - 1 },
                cv::Scalar::all(0), cv::FILLED);
  cv::rectangle(_faceTexture, { Size / 2, Size / 2 }, { Size - 1, Size - 1 },
                cv::Scalar::all(0), cv::FILLED);

  // Model space corner points (height is negative z).
  // Top:
  _cornerPoints[0] = { -1.0, -1.0, -1.0 };
  _cornerPoints[1] = {  1.0, -1.0, -1.0 };
  _cornerPoints[2] = {  1.0,  1.0, -1.0 };
  _cornerPoints[3] = { -1.0,  1.0, -1.0 };
  // Bottom:
  _cornerPoints[4] = { -1.0, -1.0,  1.0 };
  _cornerPoints[5] = {  1.0, -1.0,  1.0 };
  _cornerPoints[6] = {  1.0,  1.0,  1.0 };
  _cornerPoints[7] = { -1.0,  1.0,  1.0 };

  // Faces with indices to the corner points.
  _faces[0] = { 0, 1, 2, 3 };
  _faces[1] = { 3, 2, 6, 7 };
  _faces[2] = { 2, 1, 5, 6 };
  _faces[3] = { 1, 0, 4, 5 };
  _faces[4] = { 0, 3, 7, 4 };
  _faces[5] = { 7, 6, 5, 4 };

  setTransform(0.0, 1.0, -5.0);
}

void ChessboardModel::setTransform(double x, double y, double z)
{
  _modelMatrix.at<double>(0, 3) = x;
  _modelMatrix.at<double>(1, 3) = y;
  _modelMatrix.at<double>(2, 3) = z;
}

void ChessboardModel::render(cv::Mat& image, const Transformer& t) const
{
  for (const Face& face : _faces) {
    renderFace(image, face, t);
  }
}

void ChessboardModel::renderFace(cv::Mat& image, const Face& face, const Transformer& t) const
{
  if (normalVector(face, t).dot(cameraVector(face, t)) > 0.0) {
    // Only render visible faces.

    std::vector<cv::Point2d> imagePoints;
    t.projectPoints( {
      _cornerPoints[face.p0]
      , _cornerPoints[face.p1]
      , _cornerPoints[face.p2]
      , _cornerPoints[face.p3]
    }, imagePoints);

    const std::vector<cv::Point2d> textureCorners =
      { { 0.0       , 0.0 }
      , { Size - 1.0, 0.0}
      , { Size - 1.0, Size - 1.0 }
      , { 0.0       , Size - 1.0 }
      };

    const cv::Mat H = cv::findHomography(textureCorners, imagePoints);
    if (!H.empty()) {
      cv::warpPerspective(_faceTexture, image, H, image.size(),
                          cv::INTER_LINEAR, cv::BORDER_TRANSPARENT);
    }
  }
}

void ChessboardModel::renderDebug(cv::Mat& image, const Transformer& t) const
{
  for (const Face& face : _faces) {
    std::vector<cv::Point3d> objectPoints =
      { _cornerPoints[face.p0]
      , _cornerPoints[face.p1]
      , _cornerPoints[face.p2]
      , _cornerPoints[face.p3]
      };
    std::vector<cv::Point2d> imagePointsd;
    t.projectPoints(objectPoints, imagePointsd);

    std::vector<cv::Point2i> imagePointsi(points2i(imagePointsd));

    cv::Vec3d normal = normalVector(face, t);
    cv::Point3d normReach = t.modelMatrixTransform(_cornerPoints[face.p0]) + cv::Point3d(normal);

    std::vector<cv::Point2d> normalPointsd;
    t.projectPoints({_cornerPoints[face.p0], normReach}, normalPointsd);

    std::vector<cv::Point2i> normalPointsi(points2i(normalPointsd));
    cv::line(image, normalPointsi[0], normalPointsi[1], cv::Scalar(0, 255, 255));

    cv::Vec3d camera = cameraVector(face, t);

    //cv::Point3d cameraReach = _cornerPoints[face.p0] + cv::Point3d(camera);

    //std::vector<cv::Point2d> cameraPointsd;
    //t.projectPoints({_cornerPoints[face.p0], cameraReach}, cameraPointsd);

    cv::Scalar camViewCol = normal.dot(camera) > 0 ? cv::Scalar(0, 255, 0) : cv::Scalar(0, 0, 255);
    cv::polylines(image, imagePointsi, true, camViewCol);

    //std::vector<cv::Point2i> cameraPointsi(points2i(cameraPointsd));
    //cv::line(image, cameraPointsi[0], cameraPointsi[1], camViewCol, 20);
  }
}

cv::Vec3d ChessboardModel::normalVector(const Face& face, const Transformer& t) const
{
  const cv::Point3d p0(t.modelMatrixTransform(_cornerPoints[face.p0]));
  const cv::Point3d p1(t.modelMatrixTransform(_cornerPoints[face.p1]));
  const cv::Point3d p3(t.modelMatrixTransform(_cornerPoints[face.p3]));

  const cv::Vec3d to1(p1 - p0);
  const cv::Vec3d to3(p3 - p0);

  return cv::normalize(to3.cross(to1));
}

cv::Vec3d ChessboardModel::cameraVector(const Face& face, const Transformer& t) const
{
  const cv::Vec3d cameraVector(t.centerOfProjection() - t.modelMatrixTransform(_cornerPoints[face.p0]));
  return cv::normalize(cameraVector);
}

void ChessboardModel::renderFace2(cv::Mat& image, const std::vector<cv::Point2d>& dstPoints) const
{
  std::vector<cv::Point2f> srcPoints =
    { { 0.0,        0.0 }
    , { Size - 1.0, 0.0}
    , { Size - 1.0, Size - 1.0}
    , { 0.0,        Size - 1.0 }
    };
  cv::Mat H = cv::findHomography(srcPoints, dstPoints);
  if (!H.empty()) {
    cv::warpPerspective(_faceTexture, image, H, image.size(),
                        cv::INTER_LINEAR, cv::BORDER_TRANSPARENT);
  }
}
