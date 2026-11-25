add_executable(pa3 main.cpp rasterizer.cpp Triangle.cpp)
target_link_libraries(pa3 PRIVATE
    Eigen3::Eigen
    opencv_core opencv_imgproc opencv_imgcodecs opencv_highgui)
target_compile_definitions(pa3 PRIVATE
    GAMES101_MODELS_DIR="${CMAKE_CURRENT_SOURCE_DIR}/models/")
games101_enable_warnings(pa3)
