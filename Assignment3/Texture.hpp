#pragma once

#include "global.hpp"

#include <Eigen/Eigen>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <string>

class Texture
{
  private:
    cv::Mat image_data;

  public:
    int width = 0;
    int height = 0;

    Texture() = default;

    explicit Texture(const std::string& name)
    {
        image_data = cv::imread(name);
        cv::cvtColor(image_data, image_data, cv::COLOR_RGB2BGR);
        width = image_data.cols;
        height = image_data.rows;
    }

    [[nodiscard]] Eigen::Vector3f getColor(float u, float v) const
    {
        auto u_img = static_cast<int>(u * static_cast<float>(width));
        auto v_img = static_cast<int>((1.0f - v) * static_cast<float>(height));
        auto color = image_data.at<cv::Vec3b>(v_img, u_img);
        return Eigen::Vector3f{static_cast<float>(color[0]), static_cast<float>(color[1]),
                               static_cast<float>(color[2])};
    }
};
