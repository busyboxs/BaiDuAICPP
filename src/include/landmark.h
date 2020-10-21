#pragma once
#include "util.h"

struct LandmarkInfo {
    std::string landmark;

    void print() {
        std::cout << std::setw(30) << std::setfill('-') << '\n';
        std::cout << "landmark: " << landmark << '\n';
    }
};

class Landmark
{
public:
    Landmark();
    ~Landmark();

    Json::Value request(std::string imgBase64, std::map<std::string, std::string>& options);

    // only get first result
    void getResult(LandmarkInfo& result);


private:
    Json::Value obj_;
    std::string url_;
    // file to save token key
    std::string filename_;
};

void landmarkTest();