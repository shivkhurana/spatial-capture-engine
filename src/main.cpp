#include "spatial_detector.h"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <input_image> <output_image>" << std::endl;
        return 1;
    }

    std::string inputPath = argv[1];
    std::string outputPath = argv[2];

    cv::Mat image = cv::imread(inputPath);
    if (image.empty()) {
        std::cerr << "Failed to load image: " << inputPath << std::endl;
        return 1;
    }

    SpatialDetector detector;
    auto boxes = detector.detectRectangles(image);

    // Draw bounding boxes
    for (const auto& box : boxes) {
        cv::rectangle(image, cv::Point(box.x, box.y), cv::Point(box.x + box.width, box.y + box.height), cv::Scalar(0, 255, 0), 2);
    }

    if (!cv::imwrite(outputPath, image)) {
        std::cerr << "Failed to save image: " << outputPath << std::endl;
        return 1;
    }

    std::cout << "Processed " << boxes.size() << " rectangles." << std::endl;
    return 0;
}