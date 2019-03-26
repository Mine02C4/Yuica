#include "grid_image.hpp"

#include <iostream>

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include "ui_util.hpp"

namespace yuica {

namespace fs = std::filesystem;

void GridImage::LoadImage(fs::path filepath) {
  using namespace cv;
  source_image_ = imread(filepath.string());
  Mat img2, mask, mini_image;
  Mat bgdModel, fgdModel;
  // double image_ratio = 0.2;
  // resize(source_image_, mini_image, Size(), image_ratio, image_ratio);
  // int img_width = mini_image.size().width;
  // int img_height = mini_image.size().height;
  // Rect rect(img_width * 0.05, img_height * 0.02, img_width * 0.9,
  //          img_height * 0.96);
  // grabCut(mini_image, img2, rect, bgdModel, fgdModel, 1, GC_INIT_WITH_RECT);
  // compare(img2, GC_PR_FGD, mask, CMP_EQ);
  // mini_image.copyTo(img2, mask);
  // ShowImageFixedWidth("Original Image", source_image_, 800);
  // ShowImageFixedWidth("Masked Image", img2, 800);
  // waitKey(0);
  image_loaded_ = true;
}

void GridImage::DetectCards() {
  using namespace cv;
  if (!image_loaded_) {
    throw new std::exception("Image has not been loaded.");
  }
  double process_image_ratio = 0.5;
  Mat canny_src, mini_image, grab_src;
  resize(source_image_, mini_image, Size(), process_image_ratio,
         process_image_ratio);
  blur(mini_image, canny_src, Size(9, 9));
  Mat canny_out;
  double canny_threshold1 = 1.0;
  double canny_threshold2 = 30.0;
  Canny(canny_src, canny_out, canny_threshold1, canny_threshold2);
  // waitKey(1);
  // return;
  dilate(canny_out, grab_src, Mat(), Point(), 1);
  // ShowImageFixedWidth("Canny out", grab_src, 1200);
  Mat label_image, stats, centroids;
  int labels =
      connectedComponentsWithStats(grab_src, label_image, stats, centroids);
  for (int i = 1; i < labels; ++i) {
    auto param = stats.ptr<int>(i);
    int x = param[ConnectedComponentsTypes::CC_STAT_LEFT];
    int y = param[ConnectedComponentsTypes::CC_STAT_TOP];
    int width = param[ConnectedComponentsTypes::CC_STAT_WIDTH];
    int height = param[ConnectedComponentsTypes::CC_STAT_HEIGHT];
    if (height > mini_image.size().height * 0.4 &&
        height < mini_image.size().height * 0.8) {
      rectangle(mini_image, Rect(x, y, width, height), Scalar(0, 255, 0), 2);
      std::vector<Point> contours;
      for (int j = 0; j < height; ++j) {
        for (int k = 0; k < width; ++k) {
          if (label_image.at<int>(y + j, x + k) == i) {
            contours.push_back(Point(x + k, y + j));
          }
        }
      }
      std::vector<std::vector<Point>> hull(1);
      convexHull(contours, hull[0]);
      std::vector<Point> approx;
      approxPolyDP(hull[0], approx, arcLength(hull[0], true) * 0.001, true);
      hull[0] = approx;
      approxPolyDP(hull[0], approx, arcLength(hull[0], true) * 0.05, true);
      hull[0] = approx;
      drawContours(mini_image, hull, 0, Scalar(0, 0, 255), 3);
      for (int j = 0; j < hull[0].size(); ++j) {
        circle(mini_image, hull[0][j], 10, Scalar(120, 250, 0), -1);
        putText(mini_image, std::to_string(j), hull[0][j], FONT_HERSHEY_SIMPLEX,
                1.0, Scalar(120, 250, 0), 3);
      }
      if (hull[0].size() != 4) {
        throw new std::exception("Wrong detection. It is not a card.");
      }
      Point2f src[4];
      int offset = 0;
      for (int j = 0; j < 4; ++j) {
        auto point = hull[0][j];
        if (point.x < x + width / 2 && point.y < y + height / 2) {
          offset = j;
          break;
        }
      }
      for (int j = 0; j < 4; ++j) {
        int id = (offset + j) % 4;
        src[j] = Point2f(hull[0][id]) / process_image_ratio;
      }
      Mat card(4000, 3000, CV_8UC3);
      Point2f dst[4] = {{0, 0}, {3000, 0}, {3000, 4000}, {0, 4000}};
      Mat m = getPerspectiveTransform(src, dst);
      warpPerspective(source_image_, card, m, card.size());
      card_images_.push_back(card);
      card_centroids_.push_back(
          Point2d(centroids.at<double>(i, 0) / process_image_ratio,
                  centroids.at<double>(i, 1) / process_image_ratio));
    }
  }
  ShowImageFixedWidth("Detected Cards", mini_image, 1200);
}

void GridImage::ShowCards() {
  using namespace cv;
  if (!image_loaded_) {
    throw new std::exception("Image has not been loaded.");
  }
  if (card_images_.size() <= 0) {
    throw new std::exception("No Yuica has been detected.");
  }
  if (card_images_.size() != 6) {
    std::cout << "Wrong Yuica detection : n = " << card_images_.size()
              << std::endl;
    return;
  }
  int height = card_images_[0].size().height;
  int width = card_images_[0].size().width;
  Mat view(height * 2, width * 3, CV_8UC3);
  int img_map[6];
  std::vector<std::pair<int, Point2d>> map(6);
  for (int i = 0; i < 6; ++i) {
    map[i] = std::make_pair(i, card_centroids_[i]);
  }
  int middle_height = source_image_.size().height / 2;
  std::sort(map.begin(), map.end(),
            [middle_height](std::pair<int, Point2d> a,
                            std::pair<int, Point2d> b) -> auto {
              int a_row = (a.second.y < middle_height) ? 0 : 1;
              int b_row = (b.second.y < middle_height) ? 0 : 1;
              return (a_row == b_row) ? (a.second.x < b.second.x)
                                      : (a_row < b_row);
            });
  for (int i = 0; i < 6; ++i) {
    int nx = i % 3;
    int ny = i / 3;
    Rect roi(nx * width, ny * height, width, height);
    Mat img_roi = view(roi);
    card_images_[map[i].first].copyTo(img_roi);
  }
  ShowImageFixedWidth("Cards", view, 800);
}

}  // namespace yuica
