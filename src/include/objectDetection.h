#pragma once
#include <string>
#include <iostream>
#include <opencv2/opencv.hpp>
#include "util.h"

struct ObjDetInfo {
    uint32_t left;
    uint32_t top;
    uint32_t width;
    uint32_t height;

    void print() {
        std::cout << std::setw(20) << std::setfill('-') << '\n';
        std::cout << "left: " << left << "\n";
        std::cout << "top: " << top << "\n";
        std::cout << "width: " << width << "\n";
        std::cout << "height: " << height << "\n";
    }

    void draw(cv::Mat& img) {
        cv::Rect rect(left, top, width, height);
        cv::rectangle(img, rect, cv::Scalar(255, 0, 255), 3);
    }
};

class ObjectDetection
{
public:
    ObjectDetection();
    ~ObjectDetection();

    Json::Value request(std::string imgBase64, std::map<std::string, std::string>& options);

    // only get first result
    void getResult(ObjDetInfo& result);


private:
    Json::Value obj_;
    std::string url_;
    // file to save token key
    std::string filename_;
};

void objectDetectionTest();