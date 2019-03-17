#ifndef YUICA_IMAGE_ANALYZER_GRID_IMAGE_H_
#define YUICA_IMAGE_ANALYZER_GRID_IMAGE_H_

#include <filesystem>

#include <opencv2/core.hpp>

namespace yuica {

class GridImage {
 public:
  void LoadImage(std::filesystem::path filepath);
  void DetectCards();

 private:
  bool image_loaded_ = false;
  cv::Mat source_image_;
  cv::Mat card_roi_;
  int GetSourceWidth() { return source_image_.size().width; }
  int GetSourceHeight() { return source_image_.size().height; }
};

}  // namespace yuica

#endif  // YUICA_IMAGE_ANALYZER_GRID_IMAGE_H_
