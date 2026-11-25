add_executable(pa6 main.cpp Scene.cpp BVH.cpp Renderer.cpp)
target_compile_definitions(pa6 PRIVATE
    GAMES101_MODELS_DIR="${CMAKE_CURRENT_SOURCE_DIR}/models/")
games101_enable_warnings(pa6)
