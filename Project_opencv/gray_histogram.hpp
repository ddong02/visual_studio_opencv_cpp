#ifndef GRAY_HISTOGRAM_H
#define GRAY_HISTOGRAM_H

#include <opencv2/core/mat.hpp>

cv::Mat calcGrayHist(const cv::Mat& img);
cv::Mat getGrayHistImage(const cv::Mat& hist);

#endif // GRAY_HISTOGRAM_H