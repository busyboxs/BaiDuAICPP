#pragma once
#include "util.h"

struct PlantInfo {
    std::string name;
    float score;
    std::string baikeurl;
    std::string imageurl;
    std::string baikedesc;

    void print() {
        std::cout << std::setw(30) << std::setfill('-') << '\n';
        std::cout << "name: " << name << "\n";
        std::cout << "score: " << std::fixed << std::setprecision(6) << score << "\n";
        if (baikeurl != "null")
            std::cout << "baikeurl: " << baikeurl << "\n";
        if (imageurl != "null")
            std::cout << "imageurl: " << imageurl << "\n";
        if (baikedesc != "null")
            std::cout << "baikedesc: " << baikedesc << "\n";
    }
};

class Plant
{
public:
    Plant();
    ~Plant();

    Json::Value request(std::string imgBase64, std::map<std::string, std::string>& options);

    // get all return results
    void getAllResult(std::vector<PlantInfo>& results);

    // only get first result
    void getResult(PlantInfo& result);


private:
    Json::Value obj_;
    std::string url_;
    // file to save token key
    std::string filename_;
};

void plantTest();