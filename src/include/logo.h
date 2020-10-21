#pragma once
#include "util.h"

struct Location {
    int left;
    int top;
    int width;
    int height;

    void print() {
        std::cout << "\n\t left: " << left << " top: " << top << " width: " << width << " height: " << height << '\n';
    }

    void draw(cv::Mat& img) {
        cv::Rect rect(left, top, width, height);
        cv::rectangle(img, rect, cv::Scalar(255, 0, 255), 3);
    }
};

struct LogoInfo {
    int type;
    std::string name;
    float probability;
    Location location;

    void print() {
        std::cout << std::setw(30) << std::setfill('-') << '\n';
        std::cout << "type: " << type << "\n";
        std::cout << "name: " << name << "\n";
        std::cout << "probability: " << std::fixed << std::setprecision(4) << probability << "\n";
        std::cout << "location: ";
        location.print();
    }

    void draw(cv::Mat& img) {
        location.draw(img);
    }
};

class Logo
{
public:
    Logo();
    ~Logo();

    Json::Value request(std::string imgBase64, std::map<std::string, std::string>& options);

    uint32_t getResultNum();

    // get all return results
    void getAllResult(std::vector<LogoInfo>& results);

    // only get first result
    void getResult(LogoInfo& result);


private:
    Json::Value obj_;
    std::string url_;
    uint32_t result_num_;
    // file to save token key
    std::string filename_;
};

void logoTest();