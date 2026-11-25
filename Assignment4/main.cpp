#include <chrono>
#include <cmath>
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <vector>

// State shared with the mouse callback (passed through the callback's
// userdata pointer instead of a global).
struct BezierApp
{
    std::vector<cv::Point2f> control_points;
};

void mouse_handler(int event, int x, int y, [[maybe_unused]] int flags, void* userdata)
{
    auto* app = static_cast<BezierApp*>(userdata);
    if (event == cv::EVENT_LBUTTONDOWN && app->control_points.size() < 4) {
        std::cout << "Left button of the mouse is clicked - position (" << x << ", " << y
                  << ")" << '\n';
        app->control_points.emplace_back(static_cast<float>(x), static_cast<float>(y));
    }
}

void naive_bezier(const std::vector<cv::Point2f>& points, cv::Mat& window)
{
    auto& p_0 = points[0];
    auto& p_1 = points[1];
    auto& p_2 = points[2];
    auto& p_3 = points[3];

    for (double t = 0.0; t <= 1.0; t += 0.001) {
        auto point = std::pow(1 - t, 3) * p_0 + 3 * t * std::pow(1 - t, 2) * p_1 +
                     3 * std::pow(t, 2) * (1 - t) * p_2 + std::pow(t, 3) * p_3;

        window.at<cv::Vec3b>(static_cast<int>(point.y), static_cast<int>(point.x))[2] = 255;
    }
}

cv::Point2f recursive_bezier(const std::vector<cv::Point2f>& control_points, float t)
{
    // TODO: Implement de Casteljau's algorithm
    return cv::Point2f();
}

void bezier(const std::vector<cv::Point2f>& control_points, cv::Mat& window)
{
    // TODO: Iterate through all t = 0 to t = 1 with small steps, and call de Casteljau's
    // recursive Bezier algorithm.
}

int main()
{
    BezierApp app;

    cv::Mat window = cv::Mat(700, 700, CV_8UC3, cv::Scalar(0));
    cv::cvtColor(window, window, cv::COLOR_BGR2RGB);
    cv::namedWindow("Bezier Curve", cv::WINDOW_AUTOSIZE);

    cv::setMouseCallback("Bezier Curve", mouse_handler, &app);

    int key = -1;
    while (key != 27) {
        for (auto& point : app.control_points) {
            cv::circle(window, point, 3, {255, 255, 255}, 3);
        }

        if (app.control_points.size() == 4) {
            naive_bezier(app.control_points, window);
            //   bezier(app.control_points, window);

            cv::imshow("Bezier Curve", window);
            cv::imwrite("my_bezier_curve.png", window);
            key = cv::waitKey(0);

            return 0;
        }

        cv::imshow("Bezier Curve", window);
        key = cv::waitKey(20);
    }

    return 0;
}
