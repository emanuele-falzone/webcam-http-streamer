#ifndef CAMERA_H
#define CAMERA_H

#include <opencv2/highgui/highgui.hpp>
#include <mutex>

/**
 * @brief The Camera handler class
 * Handle an OpenCV VideoCapture object in order to capture frames and convert them in jpg
 */
class Camera {
private:
    std::mutex l;
    cv::VideoCapture c;
    cv::Mat get_frame() { 
        cv::Mat m; 
        l.lock(); 
        c >> m; 
        l.unlock(); 
        return m; 
    }

public:
    Camera(int n) : c(n) {}
    Camera(std::string n) : c(n) {}
    ~Camera() { c.release(); }
    std::string jpg_encode() {
        std::vector<uchar> buf;
        cv::imencode(".jpg", get_frame(), buf);
        return std::string(buf.begin(), buf.end());
    }
};

#endif // CAMERA_H