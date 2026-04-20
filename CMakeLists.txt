cmake_minimum_required(VERSION 3.10)
project(SpatialCaptureEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find OpenCV
find_package(OpenCV REQUIRED)
if(NOT OpenCV_FOUND)
    message(FATAL_ERROR "OpenCV not found. Please install OpenCV.")
endif()

# Include directories
include_directories(include)
include_directories(${OpenCV_INCLUDE_DIRS})

# Source files
set(SOURCES
    src/main.cpp
    src/spatial_detector.cpp
)

# Executable
add_executable(spatial_capture ${SOURCES})

# Link libraries
target_link_libraries(spatial_capture ${OpenCV_LIBS})

# Compiler flags for optimization
target_compile_options(spatial_capture PRIVATE -O2 -Wall -Wextra)