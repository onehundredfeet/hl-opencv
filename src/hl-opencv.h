#include <opencv2/opencv.hpp>


void contourTest() {
   cv::Mat src;
   src.create(256, 256,  CV_8UC4);
  cv::Mat src_gray;
  cv::Mat src_hsv;
  int thresh = 100;

  cv::cvtColor( src, src_gray, cv::COLOR_BGR2GRAY );
  cv::Mat thresholded;

  // Convert from BGR to HSV colorspace
  cv::cvtColor(src, src_hsv, cv::COLOR_BGR2HSV);
  // Detect the object based on HSV Range Values
  cv::Mat src_threshold;

  int low_H = 0, low_S = 0, low_V = 0;
  const int max_value_H = 360/2;
  const int max_value = 255;
  int high_H = max_value_H;
  int high_S = max_value;
  int high_V = max_value;

  cv::inRange(src_hsv, cv::Scalar(low_H, low_S, low_V), cv::Scalar(high_H, high_S, high_V), src_threshold);

    /* 0: Binary
     1: Binary Inverted
     2: Threshold Truncated
     3: Threshold to Zero
     4: Threshold to Zero Inverted
    */
  cv::threshold( src_gray, thresholded, thresh, 255, cv::THRESH_BINARY);

  std::vector<std::vector<cv::Point> > contours;
  std::vector<cv::Vec4i> hierarchy;

  cv::findContours( thresholded, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE );

}