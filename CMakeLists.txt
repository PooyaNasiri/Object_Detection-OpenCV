cmake_minimum_required(VERSION 3.10)
project(ObjectDetectionCV25)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find OpenCV
find_package(OpenCV REQUIRED)
include_directories(${OPENCV_INCLUDE_DIRS})

# Include headers
include_directories(include ${OpenCV_INCLUDE_DIRS})

# Source files
file(GLOB SOURCES "*.cpp")

# Executable
add_executable(cv ${SOURCES})

# Link OpenCV
target_link_libraries(cv ${OpenCV_LIBS})
