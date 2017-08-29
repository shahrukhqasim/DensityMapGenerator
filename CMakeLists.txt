cmake_minimum_required(VERSION 3.5)
project(DensityMap)

# Find boost packages
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
FIND_PACKAGE(Boost 1.40 COMPONENTS regex program_options REQUIRED )

link_directories(/usr/local/lib/ /usr/lib/ ${Boost_LIBRARY_DIR})
include_directories(/usr/local/include/ DocProc/src/ ${Boost_INCLUDE_DIR})

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES src/main.cc src/DensityMap.cc src/DensityMap.h)
add_executable(DensityMap ${SOURCE_FILES})
add_executable(DensityMapHeapTest src/main_test_heap.cc)
add_executable(DensityMapGaussianTest src/main_gaussian_test.cc)
add_executable(DensityMapBinOutTest src/main_load_bin.cc)


add_executable(DensityMap2 src/main2.cc src/DensityMap2.cc src/DensityMap2.h)

target_link_libraries(DensityMap lept tesseract opencv_core opencv_imgproc opencv_highgui opencv_imgcodecs pthread gomp ${Boost_LIBRARIES} docproc)
target_link_libraries(DensityMap2 lept tesseract opencv_core opencv_imgproc opencv_highgui opencv_imgcodecs pthread gomp ${Boost_LIBRARIES} docproc)
target_link_libraries(DensityMapGaussianTest lept tesseract opencv_core opencv_imgproc opencv_highgui opencv_imgcodecs pthread gomp ${Boost_LIBRARIES} docproc)