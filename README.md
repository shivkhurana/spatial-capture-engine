# Spatial Capture Engine

An automated spatial detection system for processing noisy, imperfect real-world image data to detect and draw bounding boxes around rectangular spatial regions.

## Architecture

### Memory Control
- **RAII Principle**: All resources are managed using RAII (Resource Acquisition Is Initialization). OpenCV's `cv::Mat` handles memory automatically, preventing leaks.
- **No Raw Pointers**: The code avoids raw pointers; uses smart pointers where necessary (though not extensively needed here).
- **Exception Safety**: Try-catch blocks ensure resources are released even on errors.
- **Optimization**: Images are processed in-place where possible, and temporary objects are scoped to minimize memory usage.

### Edge-Case Handling
- **Empty Images**: Checks `image.empty()` and `image.rows == 0 || image.cols == 0`.
- **Corrupted Images**: Wrapped in try-catch for OpenCV exceptions (e.g., invalid file formats).
- **Completely Black Images**: Uses `cv::countNonZero()` to detect all-black frames.
- **Invalid Operations**: Contour detection and approximation are checked for validity (e.g., 4-sided polygons, convexity).

The system gracefully handles failures by logging errors and returning empty results, ensuring no crashes or memory leaks.

## Project Structure
- `src/`: C++ source files.
- `include/`: C++ header files.
- `scripts/`: Python testing scripts.
- `build/`: Build directory for CMake.

## Building
1. Ensure OpenCV is installed (e.g., `sudo apt install libopencv-dev` on Linux).
2. `cd build && cmake .. && make`

## Testing
1. Install Python dependencies: `pip install -r requirements.txt`
2. Run tests: `cd scripts && pytest test_automation.py`

The test suite generates mock images (valid, empty, black, corrupted), runs the binary, and logs execution times and crash reports to verify reliability.

## Usage
`./build/spatial_capture <input_image> <output_image>`

Draws green bounding boxes around detected rectangles and saves the result.