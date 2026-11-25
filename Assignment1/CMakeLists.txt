add_executable(pa1 main.cpp rasterizer.cpp Triangle.cpp)
target_link_libraries(pa1 PRIVATE
    Eigen3::Eigen
    opencv_core opencv_imgproc opencv_imgcodecs opencv_highgui)
games101_enable_warnings(pa1)
