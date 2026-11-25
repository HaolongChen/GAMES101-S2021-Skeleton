add_executable(pa2 main.cpp rasterizer.cpp Triangle.cpp)
target_link_libraries(pa2 PRIVATE
    Eigen3::Eigen
    opencv_core opencv_imgproc opencv_imgcodecs opencv_highgui)
games101_enable_warnings(pa2)
