#define uchar cvuchar
#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#undef uchar
#include "hl-idl-helpers.hpp"

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

    void getPoint(int i, int ptIdx, _hl_int2 *outpt){
      auto &x = _contours[i][ptIdx];
      outpt->x = x.x;
      outpt->y = x.y;
    }

    void getHierarchy(int i,  _hl_int4 *outpt){
      auto &x = _hierarchy[i];
      outpt->x = x[0];
      outpt->y = x[1];
      outpt->z = x[2];
      outpt->w = x[3];
    }



};

inline cv::Mat *opencl_mat_make_ref(int width, int height, int format, void *data, int stride) {
  return new cv::Mat(width, height, format, data, stride);
} 

inline cv::Scalar float4ToScalar8U( _hl_float4 *v) {
  return cv::Scalar((uint8_t)(v->x + 0.5f), (uint8_t)(v->y + 0.5f), (uint8_t)(v->z + 0.5f), (uint8_t)(v->w + 0.5f));
}
inline cv::Scalar float4ToScalar16U( _hl_float4 *v) {
  return cv::Scalar((uint16_t)(v->x + 0.5f), (uint16_t)(v->y + 0.5f), (uint16_t)(v->z + 0.5f), (uint16_t)(v->w + 0.5f));
}

inline void opencl_inRange( cv::Mat &in, cv::Mat &out, _hl_float4 *lower, _hl_float4 *upper) {
  cv::inRange( in, float4ToScalar8U(lower), float4ToScalar8U(upper), out);
}

inline Contours *opencl_find_contours( cv::Mat &in, cv::RetrievalModes retrival,cv::ContourApproximationModes approximation) {

  auto *c = new Contours();

  cv::findContours(in, c->_contours, c->_hierarchy, retrival, approximation);
  return c;
}

inline void opencl_draw_contours( cv::Mat &out, Contours *contours, int cidx, _hl_float4 *colour, int thickness, cv::LineTypes lineType, int maxLevel) {

  auto col = float4ToScalar8U(colour);

  cv::drawContours(out, contours->_contours, cidx, col, thickness, lineType, contours->_hierarchy, maxLevel);
}

inline cv::Mat *opencv_mat_make_zeroes( int r, int c, int format ) {
  auto *mp = new cv::Mat();
  *mp = cv::Mat::zeros(r, c, format );
  return mp;
}


inline void opencv_mat_zero( cv::Mat *m ) {
  *m = cv::Mat::zeros(m->rows, m->cols, m->type() );
}

inline void opencv_circle(cv::Mat *m, _hl_int2 *p, int radius, _hl_float4 *colour, int thickness, cv::LineTypes lineType) {
  cv::Point center;
  center.x = p->x;
  center.y = p->y;
  
  auto d = m->depth();
  switch(d) {
    case CV_8U: cv::circle(*m, center, radius, float4ToScalar8U(colour), thickness, lineType ); break;
    case CV_16U: cv::circle(*m, center, radius, float4ToScalar16U(colour), thickness, lineType ); break;
    case CV_32F: cv::circle(*m, center, radius, cv::Scalar(colour->x, colour->y, colour->z, colour->w), thickness, lineType ); break;
    default: 
    cv::circle(*m, center, radius, float4ToScalar8U(colour), thickness, lineType ); break;
    break;
  }
}

inline void opencv_fill_poly(cv::Mat *m, _hl_int2 *points, int numPoints, _hl_float4 *colour, cv::LineTypes lineType ) {
   auto d = m->depth();
  switch(d) {
    case CV_8U: cv::fillPoly(*m,(const cv::Point **)&points, &numPoints, 1,  float4ToScalar8U(colour), lineType ); break;
    case CV_16U: cv::fillPoly(*m,(const cv::Point **)&points, &numPoints, 1, float4ToScalar16U(colour), lineType ); break;
    case CV_32F: cv::fillPoly(*m,(const cv::Point **)&points, &numPoints, 1,   cv::Scalar(colour->x, colour->y, colour->z, colour->w), lineType ); break;
    default: 
      cv::fillPoly(*m,(const cv::Point **)&points, &numPoints, 1,  float4ToScalar8U(colour), lineType ); break; 
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