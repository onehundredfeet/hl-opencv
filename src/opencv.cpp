#ifdef EMSCRIPTEN

#include <emscripten.h>
#define HL_PRIM
#define HL_NAME(n)	EMSCRIPTEN_KEEPALIVE eb_##n
#define DEFINE_PRIM(ret, name, args)
#define _OPT(t) t*
#define _GET_OPT(value,t) *value
#define alloc_ref(r, _) r
#define alloc_ref_const(r,_) r
#define _ref(t)			t
#define _unref(v)		v
#define free_ref(v) delete (v)
#define HL_CONST const

#else

#define HL_NAME(x) opencv_##x
#include <hl.h>

// Need to link in helpers
HL_API hl_type hltx_ui16;
HL_API hl_type hltx_ui8;

#define _IDL _BYTES
#define _OPT(t) vdynamic *
#define _GET_OPT(value,t) (value)->v.t
template <typename T> struct pref {
	void (*finalize)( pref<T> * );
	T *value;
};

#define _ref(t) pref<t>
#define _unref(v) v->value
#define _unref_ptr_safe(v) (v != nullptr ? v->value : nullptr)
#define alloc_ref(r,t) _alloc_ref(r,finalize_##t)
#define alloc_ref_const(r, _) _alloc_const(r)
#define HL_CONST

template<typename T> void free_ref( pref<T> *r ) {
	if( !r->finalize ) hl_error("delete() is not allowed on const value.");
	delete r->value;
	r->value = NULL;
	r->finalize = NULL;
}

template<typename T> void free_ref( pref<T> *r, void (*deleteFunc)(T*) ) {
	if( !r->finalize ) hl_error("delete() is not allowed on const value.");
	deleteFunc( r->value );
	r->value = NULL;
	r->finalize = NULL;
}

// Float vector
struct _hl_float2 {
	float x;
	float y;
};

struct _hl_float3 {
	float x;
	float y;
	float z;
};

struct _hl_float4 {
	float x;
	float y;
	float z;
	float w;
};

// int vector
struct _hl_int2 {
	int x;
	int y;
};

struct _hl_int3 {
	int x;
	int y;
	int z;
};

struct _hl_int4 {
	int x;
	int y;
	int z;
	int w;
};

// double vector
struct _hl_double2 {
	double x;
	double y;
};

struct _hl_double3 {
	double x;
	double y;
	double z;
};

struct _hl_double4 {
	double x;
	double y;
	double z;
	double w;
};

inline void testvector(_hl_float3 *v) {
  printf("v: %f %f %f\n", v->x, v->y, v->z);
}
template<typename T> pref<T> *_alloc_ref( T *value, void (*finalize)( pref<T> * ) ) {
	if (value == nullptr) return nullptr;
	pref<T> *r = (pref<T>*)hl_gc_alloc_finalizer(sizeof(pref<T>));
	r->finalize = finalize;
	r->value = value;
	return r;
}

template<typename T> pref<T> *_alloc_const( const T *value ) {
	if (value == nullptr) return nullptr;
	pref<T> *r = (pref<T>*)hl_gc_alloc_noptr(sizeof(pref<T>));
	r->finalize = NULL;
	r->value = (T*)value;
	return r;
}

inline static varray* _idc_alloc_array(float *src, int count) {
	if (src == nullptr) return nullptr;

	varray *a = NULL;
	float *p;
	a = hl_alloc_array(&hlt_f32, count);
	p = hl_aptr(a, float);

	for (int i = 0; i < count; i++) {
		p[i] = src[i];
	}
	return a;
}
inline static varray* _idc_alloc_array(unsigned char *src, int count) {
	if (src == nullptr) return nullptr;

	varray *a = NULL;
	float *p;
	a = hl_alloc_array(&hltx_ui8, count);
	p = hl_aptr(a, float);

	for (int i = 0; i < count; i++) {
		p[i] = src[i];
	}
	return a;
}

inline static varray* _idc_alloc_array( char *src, int count) {
	return _idc_alloc_array((unsigned char *)src, count);
}

inline static varray* _idc_alloc_array(int *src, int count) {
	if (src == nullptr) return nullptr;

	varray *a = NULL;
	int *p;
	a = hl_alloc_array(&hlt_i32, count);
	p = hl_aptr(a, int);

	for (int i = 0; i < count; i++) {
		p[i] = src[i];
	}
	return a;

}

inline static varray* _idc_alloc_array(double *src, int count) {
	if (src == nullptr) return nullptr;

	varray *a = NULL;
	double *p;
	a = hl_alloc_array(&hlt_f64, count);
	p = hl_aptr(a, double);

	for (int i = 0; i < count; i++) {
		p[i] = src[i];
	}
	return a;
}


inline static varray* _idc_alloc_array(const unsigned short *src, int count) {
	if (src == nullptr) return nullptr;

	varray *a = NULL;
	unsigned short *p;
	a = hl_alloc_array(&hltx_ui16, count);
	p = hl_aptr(a, unsigned short);

	for (int i = 0; i < count; i++) {
		p[i] = src[i];
	}
	return a;
}

inline static varray* _idc_alloc_array(unsigned short *src, int count) {
	if (src == nullptr) return nullptr;

	varray *a = NULL;
	unsigned short *p;
	a = hl_alloc_array(&hltx_ui16, count);
	p = hl_aptr(a, unsigned short);

	for (int i = 0; i < count; i++) {
		p[i] = src[i];
	}
	return a;
}

inline static void _idc_copy_array( float *dst, varray *src, int count) {
	float *p = hl_aptr(src, float);
	for (int i = 0; i < count; i++) {
		dst[i] = p[i];
	}
}

inline static void _idc_copy_array( varray *dst, float *src,  int count) {
	float *p = hl_aptr(dst, float);
	for (int i = 0; i < count; i++) {
		p[i] = src[i];
	}
}


inline static void _idc_copy_array( int *dst, varray *src, int count) {
	int *p = hl_aptr(src, int);
	for (int i = 0; i < count; i++) {
		dst[i] = p[i];
	}
}

inline static void _idc_copy_array( unsigned short *dst, varray *src) {
	unsigned short *p = hl_aptr(src, unsigned short);
	for (int i = 0; i < src->size; i++) {
		dst[i] = p[i];
	}
}

inline static void _idc_copy_array( const unsigned short *cdst, varray *src) {
	unsigned short *p = hl_aptr(src, unsigned short);
	unsigned short *dst = (unsigned short *)cdst;
	for (int i = 0; i < src->size; i++) {
		dst[i] = p[i];
	}
}

inline static void _idc_copy_array( varray *dst, int *src,  int count) {
	int *p = hl_aptr(dst, int);
	for (int i = 0; i < count; i++) {
		p[i] = src[i];
	}
}


inline static void _idc_copy_array( double *dst, varray *src, int count) {
	double *p = hl_aptr(src, double);
	for (int i = 0; i < count; i++) {
		dst[i] = p[i];
	}
}

inline static void _idc_copy_array( varray *dst, double *src,  int count) {
	double *p = hl_aptr(dst, double);
	for (int i = 0; i < count; i++) {
		p[i] = src[i];
	}
}

#endif

#ifdef _WIN32
#pragma warning(disable:4305)
#pragma warning(disable:4244)
#pragma warning(disable:4316)
#endif


#define uchar uchar16
#include <hl.h>
#undef uchar
#include "hl-opencv.h"





extern "C" {

static Format Format__values[] = { CV_8UC1,CV_8UC2,CV_8UC3,CV_8UC4 };
HL_PRIM int HL_NAME(Format_toValue0)( int idx ) {
	return Format__values[idx];
}
DEFINE_PRIM(_I32, Format_toValue0, _I32);
HL_PRIM int HL_NAME(Format_indexToValue0)( int idx ) {
	return Format__values[idx];
}
DEFINE_PRIM(_I32, Format_indexToValue0, _I32);
HL_PRIM int HL_NAME(Format_valueToIndex0)( int value ) {
	for( int i = 0; i < 4; i++ ) if ( value == (int)Format__values[i]) return i; return -1;
}
DEFINE_PRIM(_I32, Format_valueToIndex0, _I32);
static cv::ColorConversionCodes ColorConversionCodes__values[] = { cv::COLOR_BGR2BGRA,cv::COLOR_RGB2RGBA,cv::COLOR_BGRA2BGR,cv::COLOR_RGBA2RGB,cv::COLOR_BGR2RGBA,cv::COLOR_RGB2BGRA,cv::COLOR_RGBA2BGR,cv::COLOR_BGRA2RGB,cv::COLOR_BGR2RGB,cv::COLOR_RGB2BGR,cv::COLOR_BGRA2RGBA,cv::COLOR_RGBA2BGRA,cv::COLOR_BGR2GRAY,cv::COLOR_RGB2GRAY,cv::COLOR_GRAY2BGR,cv::COLOR_GRAY2RGB,cv::COLOR_GRAY2BGRA,cv::COLOR_GRAY2RGBA,cv::COLOR_BGRA2GRAY,cv::COLOR_RGBA2GRAY,cv::COLOR_BGR2BGR565,cv::COLOR_RGB2BGR565,cv::COLOR_BGR5652BGR,cv::COLOR_BGR5652RGB,cv::COLOR_BGRA2BGR565,cv::COLOR_RGBA2BGR565,cv::COLOR_BGR5652BGRA,cv::COLOR_BGR5652RGBA,cv::COLOR_GRAY2BGR565,cv::COLOR_BGR5652GRAY,cv::COLOR_BGR2BGR555,cv::COLOR_RGB2BGR555,cv::COLOR_BGR5552BGR,cv::COLOR_BGR5552RGB,cv::COLOR_BGRA2BGR555,cv::COLOR_RGBA2BGR555,cv::COLOR_BGR5552BGRA,cv::COLOR_BGR5552RGBA,cv::COLOR_GRAY2BGR555,cv::COLOR_BGR5552GRAY,cv::COLOR_BGR2XYZ,cv::COLOR_RGB2XYZ,cv::COLOR_XYZ2BGR,cv::COLOR_XYZ2RGB,cv::COLOR_BGR2YCrCb,cv::COLOR_RGB2YCrCb,cv::COLOR_YCrCb2BGR,cv::COLOR_YCrCb2RGB,cv::COLOR_BGR2HSV,cv::COLOR_RGB2HSV,cv::COLOR_BGR2Lab,cv::COLOR_RGB2Lab,cv::COLOR_BGR2Luv,cv::COLOR_RGB2Luv,cv::COLOR_BGR2HLS,cv::COLOR_RGB2HLS,cv::COLOR_HSV2BGR,cv::COLOR_HSV2RGB,cv::COLOR_Lab2BGR,cv::COLOR_Lab2RGB,cv::COLOR_Luv2BGR,cv::COLOR_Luv2RGB,cv::COLOR_HLS2BGR,cv::COLOR_HLS2RGB,cv::COLOR_BGR2HSV_FULL,cv::COLOR_RGB2HSV_FULL,cv::COLOR_BGR2HLS_FULL,cv::COLOR_RGB2HLS_FULL,cv::COLOR_HSV2BGR_FULL,cv::COLOR_HSV2RGB_FULL,cv::COLOR_HLS2BGR_FULL,cv::COLOR_HLS2RGB_FULL,cv::COLOR_LBGR2Lab,cv::COLOR_LRGB2Lab,cv::COLOR_LBGR2Luv,cv::COLOR_LRGB2Luv,cv::COLOR_Lab2LBGR,cv::COLOR_Lab2LRGB,cv::COLOR_Luv2LBGR,cv::COLOR_Luv2LRGB,cv::COLOR_BGR2YUV,cv::COLOR_RGB2YUV,cv::COLOR_YUV2BGR,cv::COLOR_YUV2RGB,cv::COLOR_YUV2RGB_NV12,cv::COLOR_YUV2BGR_NV12,cv::COLOR_YUV2RGB_NV21,cv::COLOR_YUV2BGR_NV21,cv::COLOR_YUV420sp2RGB,cv::COLOR_YUV420sp2BGR,cv::COLOR_YUV2RGBA_NV12,cv::COLOR_YUV2BGRA_NV12,cv::COLOR_YUV2RGBA_NV21,cv::COLOR_YUV2BGRA_NV21,cv::COLOR_YUV420sp2RGBA,cv::COLOR_YUV420sp2BGRA,cv::COLOR_YUV2RGB_YV12,cv::COLOR_YUV2BGR_YV12,cv::COLOR_YUV2RGB_IYUV,cv::COLOR_YUV2BGR_IYUV,cv::COLOR_YUV2RGB_I420,cv::COLOR_YUV2BGR_I420,cv::COLOR_YUV420p2RGB,cv::COLOR_YUV420p2BGR,cv::COLOR_YUV2RGBA_YV12,cv::COLOR_YUV2BGRA_YV12,cv::COLOR_YUV2RGBA_IYUV,cv::COLOR_YUV2BGRA_IYUV,cv::COLOR_YUV2RGBA_I420,cv::COLOR_YUV2BGRA_I420,cv::COLOR_YUV420p2RGBA,cv::COLOR_YUV420p2BGRA,cv::COLOR_YUV2GRAY_420,cv::COLOR_YUV2GRAY_NV21,cv::COLOR_YUV2GRAY_NV12,cv::COLOR_YUV2GRAY_YV12,cv::COLOR_YUV2GRAY_IYUV,cv::COLOR_YUV2GRAY_I420,cv::COLOR_YUV420sp2GRAY,cv::COLOR_YUV420p2GRAY,cv::COLOR_YUV2RGB_UYVY,cv::COLOR_YUV2BGR_UYVY,cv::COLOR_YUV2RGB_Y422,cv::COLOR_YUV2BGR_Y422,cv::COLOR_YUV2RGB_UYNV,cv::COLOR_YUV2BGR_UYNV,cv::COLOR_YUV2RGBA_UYVY,cv::COLOR_YUV2BGRA_UYVY,cv::COLOR_YUV2RGBA_Y422,cv::COLOR_YUV2BGRA_Y422,cv::COLOR_YUV2RGBA_UYNV,cv::COLOR_YUV2BGRA_UYNV,cv::COLOR_YUV2RGB_YUY2,cv::COLOR_YUV2BGR_YUY2,cv::COLOR_YUV2RGB_YVYU,cv::COLOR_YUV2BGR_YVYU,cv::COLOR_YUV2RGB_YUYV,cv::COLOR_YUV2BGR_YUYV,cv::COLOR_YUV2RGB_YUNV,cv::COLOR_YUV2BGR_YUNV,cv::COLOR_YUV2RGBA_YUY2,cv::COLOR_YUV2BGRA_YUY2,cv::COLOR_YUV2RGBA_YVYU,cv::COLOR_YUV2BGRA_YVYU,cv::COLOR_YUV2RGBA_YUYV,cv::COLOR_YUV2BGRA_YUYV,cv::COLOR_YUV2RGBA_YUNV,cv::COLOR_YUV2BGRA_YUNV,cv::COLOR_YUV2GRAY_UYVY,cv::COLOR_YUV2GRAY_YUY2,cv::COLOR_YUV2GRAY_Y422,cv::COLOR_YUV2GRAY_UYNV,cv::COLOR_YUV2GRAY_YVYU,cv::COLOR_YUV2GRAY_YUYV,cv::COLOR_YUV2GRAY_YUNV,cv::COLOR_RGBA2mRGBA,cv::COLOR_mRGBA2RGBA,cv::COLOR_RGB2YUV_I420,cv::COLOR_BGR2YUV_I420,cv::COLOR_RGB2YUV_IYUV,cv::COLOR_BGR2YUV_IYUV,cv::COLOR_RGBA2YUV_I420,cv::COLOR_BGRA2YUV_I420,cv::COLOR_RGBA2YUV_IYUV,cv::COLOR_BGRA2YUV_IYUV,cv::COLOR_RGB2YUV_YV12,cv::COLOR_BGR2YUV_YV12,cv::COLOR_RGBA2YUV_YV12,cv::COLOR_BGRA2YUV_YV12,cv::COLOR_BayerBG2BGR,cv::COLOR_BayerGB2BGR,cv::COLOR_BayerRG2BGR,cv::COLOR_BayerGR2BGR,cv::COLOR_BayerRGGB2BGR,cv::COLOR_BayerGRBG2BGR,cv::COLOR_BayerBGGR2BGR,cv::COLOR_BayerGBRG2BGR,cv::COLOR_BayerRGGB2RGB,cv::COLOR_BayerGRBG2RGB,cv::COLOR_BayerBGGR2RGB,cv::COLOR_BayerGBRG2RGB,cv::COLOR_BayerBG2RGB,cv::COLOR_BayerGB2RGB,cv::COLOR_BayerRG2RGB,cv::COLOR_BayerGR2RGB,cv::COLOR_BayerBG2GRAY,cv::COLOR_BayerGB2GRAY,cv::COLOR_BayerRG2GRAY,cv::COLOR_BayerGR2GRAY,cv::COLOR_BayerRGGB2GRAY,cv::COLOR_BayerGRBG2GRAY,cv::COLOR_BayerBGGR2GRAY,cv::COLOR_BayerGBRG2GRAY,cv::COLOR_BayerBG2BGR_VNG,cv::COLOR_BayerGB2BGR_VNG,cv::COLOR_BayerRG2BGR_VNG,cv::COLOR_BayerGR2BGR_VNG,cv::COLOR_BayerRGGB2BGR_VNG,cv::COLOR_BayerGRBG2BGR_VNG,cv::COLOR_BayerBGGR2BGR_VNG,cv::COLOR_BayerGBRG2BGR_VNG,cv::COLOR_BayerRGGB2RGB_VNG,cv::COLOR_BayerGRBG2RGB_VNG,cv::COLOR_BayerBGGR2RGB_VNG,cv::COLOR_BayerGBRG2RGB_VNG,cv::COLOR_BayerBG2RGB_VNG,cv::COLOR_BayerGB2RGB_VNG,cv::COLOR_BayerRG2RGB_VNG,cv::COLOR_BayerGR2RGB_VNG,cv::COLOR_BayerBG2BGR_EA,cv::COLOR_BayerGB2BGR_EA,cv::COLOR_BayerRG2BGR_EA,cv::COLOR_BayerGR2BGR_EA,cv::COLOR_BayerRGGB2BGR_EA,cv::COLOR_BayerGRBG2BGR_EA,cv::COLOR_BayerBGGR2BGR_EA,cv::COLOR_BayerGBRG2BGR_EA,cv::COLOR_BayerRGGB2RGB_EA,cv::COLOR_BayerGRBG2RGB_EA,cv::COLOR_BayerBGGR2RGB_EA,cv::COLOR_BayerGBRG2RGB_EA,cv::COLOR_BayerBG2RGB_EA,cv::COLOR_BayerGB2RGB_EA,cv::COLOR_BayerRG2RGB_EA,cv::COLOR_BayerGR2RGB_EA,cv::COLOR_BayerBG2BGRA,cv::COLOR_BayerGB2BGRA,cv::COLOR_BayerRG2BGRA,cv::COLOR_BayerGR2BGRA,cv::COLOR_BayerRGGB2BGRA,cv::COLOR_BayerGRBG2BGRA,cv::COLOR_BayerBGGR2BGRA,cv::COLOR_BayerGBRG2BGRA,cv::COLOR_BayerRGGB2RGBA,cv::COLOR_BayerGRBG2RGBA,cv::COLOR_BayerBGGR2RGBA,cv::COLOR_BayerGBRG2RGBA,cv::COLOR_BayerBG2RGBA,cv::COLOR_BayerGB2RGBA,cv::COLOR_BayerRG2RGBA,cv::COLOR_BayerGR2RGBA,cv::COLOR_COLORCVT_MAX };
HL_PRIM int HL_NAME(ColorConversionCodes_toValue0)( int idx ) {
	return ColorConversionCodes__values[idx];
}
DEFINE_PRIM(_I32, ColorConversionCodes_toValue0, _I32);
HL_PRIM int HL_NAME(ColorConversionCodes_indexToValue0)( int idx ) {
	return ColorConversionCodes__values[idx];
}
DEFINE_PRIM(_I32, ColorConversionCodes_indexToValue0, _I32);
HL_PRIM int HL_NAME(ColorConversionCodes_valueToIndex0)( int value ) {
	for( int i = 0; i < 242; i++ ) if ( value == (int)ColorConversionCodes__values[i]) return i; return -1;
}
DEFINE_PRIM(_I32, ColorConversionCodes_valueToIndex0, _I32);
static cv::ThresholdTypes ThresholdTypes__values[] = { cv::THRESH_BINARY,cv::THRESH_BINARY_INV,cv::THRESH_TRUNC,cv::THRESH_TOZERO,cv::THRESH_TOZERO_INV,cv::THRESH_MASK,cv::THRESH_OTSU,cv::THRESH_TRIANGLE };
HL_PRIM int HL_NAME(ThresholdTypes_toValue0)( int idx ) {
	return ThresholdTypes__values[idx];
}
DEFINE_PRIM(_I32, ThresholdTypes_toValue0, _I32);
HL_PRIM int HL_NAME(ThresholdTypes_indexToValue0)( int idx ) {
	return ThresholdTypes__values[idx];
}
DEFINE_PRIM(_I32, ThresholdTypes_indexToValue0, _I32);
HL_PRIM int HL_NAME(ThresholdTypes_valueToIndex0)( int value ) {
	for( int i = 0; i < 8; i++ ) if ( value == (int)ThresholdTypes__values[i]) return i; return -1;
}
DEFINE_PRIM(_I32, ThresholdTypes_valueToIndex0, _I32);
static cv::RetrievalModes RetrievalModes__values[] = { cv::RETR_EXTERNAL,cv::RETR_LIST,cv::RETR_CCOMP,cv::RETR_TREE,cv::RETR_FLOODFILL };
HL_PRIM int HL_NAME(RetrievalModes_toValue0)( int idx ) {
	return RetrievalModes__values[idx];
}
DEFINE_PRIM(_I32, RetrievalModes_toValue0, _I32);
HL_PRIM int HL_NAME(RetrievalModes_indexToValue0)( int idx ) {
	return RetrievalModes__values[idx];
}
DEFINE_PRIM(_I32, RetrievalModes_indexToValue0, _I32);
HL_PRIM int HL_NAME(RetrievalModes_valueToIndex0)( int value ) {
	for( int i = 0; i < 5; i++ ) if ( value == (int)RetrievalModes__values[i]) return i; return -1;
}
DEFINE_PRIM(_I32, RetrievalModes_valueToIndex0, _I32);
static cv::ContourApproximationModes ContourApproximationModes__values[] = { cv::CHAIN_APPROX_NONE,cv::CHAIN_APPROX_SIMPLE,cv::CHAIN_APPROX_TC89_L1,cv::CHAIN_APPROX_TC89_KCOS };
HL_PRIM int HL_NAME(ContourApproximationModes_toValue0)( int idx ) {
	return ContourApproximationModes__values[idx];
}
DEFINE_PRIM(_I32, ContourApproximationModes_toValue0, _I32);
HL_PRIM int HL_NAME(ContourApproximationModes_indexToValue0)( int idx ) {
	return ContourApproximationModes__values[idx];
}
DEFINE_PRIM(_I32, ContourApproximationModes_indexToValue0, _I32);
HL_PRIM int HL_NAME(ContourApproximationModes_valueToIndex0)( int value ) {
	for( int i = 0; i < 4; i++ ) if ( value == (int)ContourApproximationModes__values[i]) return i; return -1;
}
DEFINE_PRIM(_I32, ContourApproximationModes_valueToIndex0, _I32);
static cv::LineTypes LineTypes__values[] = { cv::FILLED,cv::LINE_4,cv::LINE_8,cv::LINE_AA };
HL_PRIM int HL_NAME(LineTypes_toValue0)( int idx ) {
	return LineTypes__values[idx];
}
DEFINE_PRIM(_I32, LineTypes_toValue0, _I32);
HL_PRIM int HL_NAME(LineTypes_indexToValue0)( int idx ) {
	return LineTypes__values[idx];
}
DEFINE_PRIM(_I32, LineTypes_indexToValue0, _I32);
HL_PRIM int HL_NAME(LineTypes_valueToIndex0)( int value ) {
	for( int i = 0; i < 4; i++ ) if ( value == (int)LineTypes__values[i]) return i; return -1;
}
DEFINE_PRIM(_I32, LineTypes_valueToIndex0, _I32);
static void finalize_Contours( _ref(Contours)* _this ) { free_ref(_this ); }
HL_PRIM void HL_NAME(Contours_delete)( _ref(Contours)* _this ) {
	free_ref(_this );
}
DEFINE_PRIM(_VOID, Contours_delete, _IDL);
static void finalize_Matrix( _ref(cv::Mat)* _this ) { free_ref(_this ); }
HL_PRIM void HL_NAME(Matrix_delete)( _ref(cv::Mat)* _this ) {
	free_ref(_this );
}
DEFINE_PRIM(_VOID, Matrix_delete, _IDL);
HL_PRIM int HL_NAME(Contours_length0)(_ref(Contours)* _this) {
	return (_unref(_this)->length());
}
DEFINE_PRIM(_I32, Contours_length0, _IDL);

HL_PRIM int HL_NAME(Contours_points1)(_ref(Contours)* _this, int i) {
	return (_unref(_this)->points(i));
}
DEFINE_PRIM(_I32, Contours_points1, _IDL _I32);

HL_PRIM void HL_NAME(Contours_getPoint3)(_ref(Contours)* _this, int i, int ptidx, _hl_int2* out) {
	(_unref(_this)->getPoint(i, ptidx, (int*)out));
}
DEFINE_PRIM(_VOID, Contours_getPoint3, _IDL _I32 _I32 _STRUCT);

HL_PRIM void HL_NAME(Contours_getHierarchy3)(_ref(Contours)* _this, int i, int ptidx, _hl_int4* out) {
	(_unref(_this)->getHierarchy(i, ptidx, (int*)out));
}
DEFINE_PRIM(_VOID, Contours_getHierarchy3, _IDL _I32 _I32 _STRUCT);

HL_PRIM _ref(cv::Mat)* HL_NAME(Matrix_new0)() {
	return alloc_ref((new cv::Mat()),Matrix);
}
DEFINE_PRIM(_IDL, Matrix_new0,);

HL_PRIM void HL_NAME(Matrix_create3)(_ref(cv::Mat)* _this, int width, int height, int format) {
	(_unref(_this)->create(width, height, Format__values[format]));
}
DEFINE_PRIM(_VOID, Matrix_create3, _IDL _I32 _I32 _I32);

HL_PRIM void HL_NAME(Matrix_convertTo2)(_ref(cv::Mat)* _this, _ref(cv::Mat)* out, int conversion) {
	(cv::cvtColor( *_unref(_this) , *_unref_ptr_safe(out), ColorConversionCodes__values[conversion]));
}
DEFINE_PRIM(_VOID, Matrix_convertTo2, _IDL _IDL _I32);

HL_PRIM void HL_NAME(Matrix_inRange33)(_ref(cv::Mat)* _this, _ref(cv::Mat)* out, _hl_float3* lower, _hl_float3* upper) {
	(opencl_inRange3( *_unref(_this) , *_unref_ptr_safe(out), (float*)lower, (float*)upper));
}
DEFINE_PRIM(_VOID, Matrix_inRange33, _IDL _IDL _STRUCT _STRUCT);

HL_PRIM void HL_NAME(Matrix_threshold4)(_ref(cv::Mat)* _this, _ref(cv::Mat)* out, double threshold, double max, int thresholdType) {
	(cv::threshold( *_unref(_this) , *_unref_ptr_safe(out), threshold, max, ThresholdTypes__values[thresholdType]));
}
DEFINE_PRIM(_VOID, Matrix_threshold4, _IDL _IDL _F64 _F64 _I32);

HL_PRIM _ref(cv::Mat)* HL_NAME(Matrix_makeReference5)(int width, int height, int format, vbyte* data, int step) {
	return alloc_ref((opencl_mat_make_ref(width, height, Format__values[format], data, step)),Matrix);
}
DEFINE_PRIM(_IDL, Matrix_makeReference5, _I32 _I32 _I32 _BYTES _I32);

HL_PRIM _ref(cv::Mat)* HL_NAME(Matrix_makeZeros3)(int rows, int columns, int format) {
	return alloc_ref((opencv_mat_make_zeroes(rows, columns, Format__values[format])),Matrix);
}
DEFINE_PRIM(_IDL, Matrix_makeZeros3, _I32 _I32 _I32);

HL_PRIM void HL_NAME(Matrix_zero0)(_ref(cv::Mat)* _this) {
	(opencv_mat_zero( _unref(_this) ));
}
DEFINE_PRIM(_VOID, Matrix_zero0, _IDL);

HL_PRIM void HL_NAME(Matrix_circle5)(_ref(cv::Mat)* _this, _hl_int2* point, int radius, _hl_float4* colour, int thickness, int lineType) {
	(opencv_circle( _unref(_this) , (int*)point, radius, (float*)colour, thickness, LineTypes__values[lineType]));
}
DEFINE_PRIM(_VOID, Matrix_circle5, _IDL _STRUCT _I32 _STRUCT _I32 _I32);

HL_PRIM void HL_NAME(Matrix_writeImage1)(_ref(cv::Mat)* _this, vstring * path) {
	const char* path__cstr = (path == nullptr) ? "" : hl_to_utf8( path->bytes ); // Should be garbage collected
	(opencv_write_image( _unref(_this) , path__cstr));
}
DEFINE_PRIM(_VOID, Matrix_writeImage1, _IDL _STRING);

HL_PRIM _ref(Contours)* HL_NAME(Matrix_findContours2)(_ref(cv::Mat)* _this, int retrival, int approximation) {
	return alloc_ref((opencl_find_contours( *_unref(_this) , RetrievalModes__values[retrival], ContourApproximationModes__values[approximation])),Contours);
}
DEFINE_PRIM(_IDL, Matrix_findContours2, _IDL _I32 _I32);

}
