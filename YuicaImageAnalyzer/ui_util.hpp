#ifndef YUICA_IMAGE_ANALYZER_UI_UTIL_H_
#define YUICA_IMAGE_ANALYZER_UI_UTIL_H_

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

static inline void ShowImageFixedWidth(cv::String winname, cv::InputArray mat,
                                       int width) {
  using namespace cv;
  Mat img;
  double ratio = (double)width / mat.size().width;
  resize(mat, img, Size(), ratio, ratio, INTER_LINEAR);
  imshow(winname, img);
}

#endif  // YUICA_IMAGE_ANALYZER_UI_UTIL_H_
