cmake_minimum_required(VERSION 3.11)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -pedantic")

message("Enabling Visual Leak Detector")
find_library(VLD vld HINTS "C:/Users/Benjamin/Desktop/LainProject/C++/VLD/Visual Leak Detector/lib/Win64/")
find_path(VLD_H vld.h HINTS "C:/Users/Benjamin/Desktop/LainProject/C++/VLD/Visual Leak Detector/include/")
get_filename_component(VLD_LIBRARY_DIR "${VLD}" PATH)
include_directories("C:/Users/Benjamin/Desktop/LainProject/C++/VLD/Visual Leak Detector/include/")
link_libraries("${VLD}")
message("${VLD_H}")
message("${VLD}")
message("Enabling Visual Leak Detector Done")

#add_compile_options(-DMATRIX_DEBUG)
add_executable(MatrixTest MatrixTest.cpp)
#target_link_libraries(MatrixTest "${VLD}")
#add_executable(MatrixProduct Matrix.h MatrixProduct.cpp)

