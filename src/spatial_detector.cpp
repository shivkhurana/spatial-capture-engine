#include "spatial_detector.h"
#include <opencv2/imgproc.hpp>
#include <iostream>

SpatialDetector::SpatialDetector() {
    // Initialization if needed
}

std::vector<BoundingBox> SpatialDetector::detectRectangles(const cv::Mat& image) {
    if (!isImageValid(image)) {
        std::cerr << "Invalid image provided." << std::endl;
        return {};
    }

    try {
        return findRectangles(image);
    } catch (const cv::Exception& e) {
        std::cerr << "OpenCV error: " << e.what() << std::endl;
        return {};
    } catch (const std::exception& e) {
        std::cerr << "Standard error: " << e.what() << std::endl;
        return {};
    }
}

bool SpatialDetector::isImageValid(const cv::Mat& image) {
    if (image.empty()) {
        std::cerr << "Image is empty." << std::endl;
        return false;
    }
    if (image.rows == 0 || image.cols == 0) {
        std::cerr << "Image has zero dimensions." << std::endl;
        return false;
    }
    // Check if completely black
    cv::Mat gray;
    if (image.channels() == 3) {
        cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    } else {
        gray = image.clone();
    }
    if (cv::countNonZero(gray) == 0) {
        std::cerr << "Image is completely black." << std::endl;
        return false;
    }
    return true;
}

std::vector<BoundingBox> SpatialDetector::findRectangles(const cv::Mat& image) {
    std::vector<BoundingBox> boxes;
    cv::Mat gray, thresh;
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    cv::threshold(gray, thresh, 127, 255, cv::THRESH_BINARY);

    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(thresh, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    for (const auto& contour : contours) {
        std::vector<cv::Point> approx;
        cv::approxPolyDP(contour, approx, 0.02 * cv::arcLength(contour, true), true);
        if (approx.size() == 4 && cv::isContourConvex(approx)) {
            cv::Rect rect = cv::boundingRect(approx);
            boxes.push_back({rect.x, rect.y, rect.width, rect.height});
        }
    }
    return boxes;
}