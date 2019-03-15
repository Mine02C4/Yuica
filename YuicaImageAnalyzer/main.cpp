#include "main.hpp"

#include <filesystem>
#include <iostream>

#include "grid_image.hpp"

namespace yuica {}

namespace fs = std::filesystem;

int main(int argc, char** argv) {
  std::cout << "Yuica Image Analyzer" << std::endl;
  yuica::GridImage grid_image;
  fs::path image_path = "input";
  image_path /= "20190315_003401.jpg";
  grid_image.LoadImage(image_path);
  return 0;
}
