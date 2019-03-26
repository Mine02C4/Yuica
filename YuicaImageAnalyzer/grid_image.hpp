#ifndef YUICA_IMAGE_ANALYZER_GRID_IMAGE_H_
#define YUICA_IMAGE_ANALYZER_GRID_IMAGE_H_

#include <filesystem>
#include <vector>

#include <opencv2/core.hpp>

namespace yuica {

class GridImage {
 public:
  void LoadImage(std::filesystem::path filepath);
  void DetectCards();
  void ShowCards();

 private:
  bool image_loaded_ = false;
  cv::Mat source_image_;
  std::vector<cv::Mat> card_images_;
  std::vector<cv::Point2d> card_centroids_;

  int GetSourceWidth() { return source_image_.size().width; }
  int GetSourceHeight() { return source_image_.size().height; }
};

}  // namespace yuica

#endif  // YUICA_IMAGE_ANALYZER_GRID_IMAGE_H_
