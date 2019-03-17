#include "grid_image.hpp"

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
  double process_image_ratio = 0.3;
  Mat canny_src, mini_image, grab_src;
  resize(source_image_, mini_image, Size(), process_image_ratio,
         process_image_ratio);
  blur(mini_image, canny_src, Size(7, 7));
  Mat canny_out;
  double canny_threshold1 = 20.0;
  double canny_threshold2 = 30.0;
  Canny(canny_src, canny_out, canny_threshold1, canny_threshold2);
  dilate(canny_out, grab_src, Mat());
  // waitKey(0);
  Mat components, stats, centroids;
  int labels =
      connectedComponentsWithStats(grab_src, components, stats, centroids);
  card_roi_ = Mat_<int>(0, 4);
  for (int i = 0; i < labels; ++i) {
    auto param = stats.ptr<int>(i);
    int x = param[ConnectedComponentsTypes::CC_STAT_LEFT];
    int y = param[ConnectedComponentsTypes::CC_STAT_TOP];
    int width = param[ConnectedComponentsTypes::CC_STAT_WIDTH];
    int height = param[ConnectedComponentsTypes::CC_STAT_HEIGHT];
    if (height > mini_image.size().height * 0.3 &&
        height < mini_image.size().height * 0.8) {
      rectangle(mini_image, Rect(x, y, width, height), Scalar(0, 255, 0), 3);
      card_roi_.push_back(Vec4d(x, y, width, height));
    }
  }
  ShowImageFixedWidth("Detected Cards", mini_image, 800);
}
}

}  // namespace yuica
