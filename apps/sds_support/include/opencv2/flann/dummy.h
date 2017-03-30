
#ifndef OPENCV_FLANN_DUMMY_H_
#define OPENCV_FLANN_DUMMY_H_

namespace cvflann
{

#if (defined WIN32 || defined _WIN32 || defined WINCE) && defined CVAPI_EXPORTS
__declspec(dllexport)
#endif
void dummyfunc();

}


#endif  /* OPENCV_FLANN_DUMMY_H_ */

// XSIP watermark, do not delete 67d7842dbbe25473c3c32b93c0da8047785f30d78e8a024de1b57352245f9689
