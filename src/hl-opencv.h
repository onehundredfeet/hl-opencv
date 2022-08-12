#define uchar cvuchar
#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#undef uchar

typedef int Format;


class Contours {
  public:
      std::vector<std::vector<cv::Point> > _contours;
    std::vector<cv::Vec4i> _hierarchy;

    int length() {
      return _contours.size();;
    }
    int points(int i ){
      return _contours[i].size();
    }

    void getPoint(int i, int ptIdx, int *outpt){
      auto &x = _contours[i][ptIdx];
      outpt[0] = x.x;
      outpt[1] = x.y;
    }

    void getHierarchy(int i, int ptIdx, int *outpt){
      auto &x = _hierarchy[i];
      outpt[0] = x[0];
      outpt[1] = x[1];
      outpt[2] = x[2];
      outpt[3] = x[3];
    }



};

inline cv::Mat *opencl_mat_make_ref(int width, int height, int format, void *data, int stride) {
  return new cv::Mat(width, height, format, data, stride);
} 

inline void opencl_inRange3( cv::Mat &in, cv::Mat &out, float *lower, float *upper) {
  cv::inRange( in, cv::Scalar(lower[0], lower[1], lower[2]), cv::Scalar(upper[0], upper[1], upper[2]), out);
}

inline Contours *opencl_find_contours( cv::Mat &in, cv::RetrievalModes retrival,cv::ContourApproximationModes approximation) {

  auto *c = new Contours();

  cv::findContours(in, c->_contours, c->_hierarchy, retrival, approximation);
  return c;
}

inline cv::Mat *opencv_mat_make_zeroes( int r, int c, int format ) {
  auto *mp = new cv::Mat();
  *mp = cv::Mat::zeros(r, c, format );
  return mp;
}


inline void opencv_mat_zero( cv::Mat *m ) {
  *m = cv::Mat::zeros(m->rows, m->cols, m->type() );
}

inline void opencv_circle(cv::Mat *m, int *p, int radius, float *colour, int thickness, cv::LineTypes lineType) {
  cv::Point center;
  center.x = p[0];
  center.y = p[1];
  
  auto d = m->depth();
  switch(d) {
    case CV_8U: cv::circle(*m, center, radius, cv::Scalar((u_int8_t)colour[0], (u_int8_t)colour[1], (u_int8_t)colour[2], (u_int8_t)colour[3]), thickness, lineType ); break;
    case CV_16U: cv::circle(*m, center, radius, cv::Scalar((u_int16_t)colour[0], (u_int16_t)colour[1], (u_int16_t)colour[2], (u_int16_t)colour[3]), thickness, lineType ); break;
    case CV_32F: cv::circle(*m, center, radius, cv::Scalar((float)colour[0], (float)colour[1], (float)colour[2], (float)colour[3]), thickness, lineType ); break;
    default: 
    cv::circle(*m, center, radius, cv::Scalar((u_int8_t)colour[0], (u_int8_t)colour[1], (u_int8_t)colour[2], (u_int8_t)colour[3]), thickness, lineType ); break;
    break;
  }
}

inline void opencv_fill_poly(cv::Mat *m, int *points, int numPoints, float *colour, cv::LineTypes lineType ) {
   auto d = m->depth();
  switch(d) {
    case CV_8U: cv::fillPoly(*m,(const cv::Point **)&points, &numPoints, 1,  cv::Scalar((u_int8_t)colour[0], (u_int8_t)colour[1], (u_int8_t)colour[2], (u_int8_t)colour[3]), lineType ); break;
    case CV_16U: cv::fillPoly(*m,(const cv::Point **)&points, &numPoints, 1, cv::Scalar((u_int16_t)colour[0], (u_int16_t)colour[1], (u_int16_t)colour[2], (u_int16_t)colour[3]), lineType ); break;
    case CV_32F: cv::fillPoly(*m,(const cv::Point **)&points, &numPoints, 1,   cv::Scalar((float)colour[0], (float)colour[1], (float)colour[2], (float)colour[3]), lineType ); break;
    default: 
      cv::fillPoly(*m,(const cv::Point **)&points, &numPoints, 1,  cv::Scalar((u_int8_t)colour[0], (u_int8_t)colour[1], (u_int8_t)colour[2], (u_int8_t)colour[3]), lineType ); break;    break;
  }

}

inline void opencv_write_image(cv::Mat *m, const char *path ) {
    bool result = false;
    try
    {
        result = cv::imwrite(path, *m);
    }
    catch (const cv::Exception& ex)
    {
        fprintf(stderr, "Exception converting image to format: %s\n", ex.what());
    }
    if (result)
        printf("Saved PNG file with alpha data.\n");
    else
        printf("ERROR: Can't save PNG file.\n");
}

#if 0
void contourTest() {
   cv::Mat src;
//   src.create(256, 256,  CV_8UC4);
   src.zeros(256, 256, CV_8UC4);
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
#endif