cmake_minimum_required(VERSION 3.25)
project(games101 LANGUAGES CXX)

# ---- Language standard: C++20 everywhere -----------------------------------
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type" FORCE)
endif()

# ---- Dependencies ------------------------------------------------------------
# Eigen and OpenCV come from vcpkg (see vcpkg.json + CMakePresets.json) or
# from the system package manager. Either way they are found through their
# CMake package configs -- no hardcoded include paths anywhere.
find_package(Eigen3 3.4 CONFIG REQUIRED)
find_package(OpenCV 4 REQUIRED COMPONENTS core imgproc imgcodecs highgui)

# ---- Shared helpers ----------------------------------------------------------
# Strict warnings for every assignment target.
function(games101_enable_warnings target)
    if(MSVC)
        target_compile_options(${target} PRIVATE /W4 /permissive-)
    else()
        target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic)
    endif()
endfunction()

# Copy a PA's model/texture assets next to its executable so the binary finds
# them no matter which directory it is launched from.
function(games101_copy_assets target source_dir)
    if(EXISTS ${source_dir})
        add_custom_command(TARGET ${target} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
                    ${source_dir} $<TARGET_FILE_DIR:${target}>/models)
    endif()
endfunction()

# ---- Assignments -------------------------------------------------------------
add_subdirectory(Assignment1)
add_subdirectory(Assignment2)
add_subdirectory(Assignment3)
add_subdirectory(Assignment4)
add_subdirectory(Assignment5)
add_subdirectory(Assignment6)
add_subdirectory(Assignment7)

# Assignment8 (CGL mass-spring viewer) is intentionally not part of the build:
# it is a separate OpenGL framework borrowed from CS184 and is deferred.
# Its original sources are kept in ./Assignment8 for reference.
