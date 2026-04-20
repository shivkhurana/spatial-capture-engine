#ifndef SPATIAL_DETECTOR_H
#define SPATIAL_DETECTOR_H

#include <opencv2/opencv.hpp>
#include <vector>
#include <memory>

struct BoundingBox {
    int x, y, width, height;
};

class SpatialDetector {
public:
    SpatialDetector();
    ~SpatialDetector() = default;

    // Process image and return bounding boxes for rectangular regions
    std::vector<BoundingBox> detectRectangles(const cv::Mat& image);

private:
    // Helper to check if image is valid
    bool isImageValid(const cv::Mat& image);

    // Helper to detect rectangles using contours
    std::vector<BoundingBox> findRectangles(const cv::Mat& image);
};

#endif // SPATIAL_DETECTOR_H