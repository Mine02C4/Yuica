#include "main.hpp"

#include <filesystem>
#include <iostream>

#include <opencv2/highgui.hpp>

#include "grid_image.hpp"

namespace yuica {}

namespace fs = std::filesystem;

int main(int argc, char** argv) {
  std::cout << "Yuica Image Analyzer" << std::endl;
  yuica::GridImage grid_image;
  fs::path image_path = "input";
  image_path /= "20190315_003401.jpg";
  grid_image.LoadImage(image_path);
  for (;;) {
    grid_image.DetectCards();
    if (cv::waitKey(0) == 27 /* Esc */) break;
  }
  return 0;
}
