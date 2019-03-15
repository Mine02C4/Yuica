#include "grid_image.hpp"

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include "ui_util.hpp"

namespace yuica {

namespace fs = std::filesystem;

void GridImage::LoadImage(fs::path filepath) {
  using namespace cv;
  Mat img = imread(filepath.string());
  ShowImageFixedWidth("Image", img, 800);
  waitKey(0);
}

}  // namespace yuica
