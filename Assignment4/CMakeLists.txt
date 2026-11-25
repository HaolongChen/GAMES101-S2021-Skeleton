add_executable(pa4 main.cpp)
target_link_libraries(pa4 PRIVATE
    opencv_core opencv_imgproc opencv_imgcodecs opencv_highgui)
games101_enable_warnings(pa4)
