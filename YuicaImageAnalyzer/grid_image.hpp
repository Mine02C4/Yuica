#ifndef YUICA_IMAGE_ANALYZER_GRID_IMAGE_H_
#define YUICA_IMAGE_ANALYZER_GRID_IMAGE_H_

#include <filesystem>

#include <opencv2/core.hpp>

namespace yuica {

class GridImage {
 public:
  void LoadImage(std::filesystem::path filepath);

 private:
};

}  // namespace yuica

#endif  // YUICA_IMAGE_ANALYZER_GRID_IMAGE_H_
