#pragma once
#include "util.h"

struct GeneralInfo {
    std::string keyword;
    float score;
    std::string root;
    std::string baikeurl;
    std::string imageurl;
    std::string baikedesc;

    void print() {
        std::cout << std::setw(30) << std::setfill('-') << '\n';
        std::cout << "keyword: " << keyword << "\n";
        std::cout << "score: " << std::fixed << std::setprecision(4) << score << "\n";
        std::cout << "root: " << root << "\n";
        if (baikeurl != "null")
            std::cout << "baikeurl: " << baikeurl << "\n";
        if (imageurl != "null")
            std::cout << "imageurl: " << imageurl << "\n";
        if (baikedesc != "null")
            std::cout << "baikedesc: " << baikedesc << "\n";
    }
};

class General
{
public:
    General();
    ~General();

    Json::Value request(std::string imgBase64, std::map<std::string, std::string>& options);

    int getResultNum();

    // get all return results
    void getAllResult(std::vector<GeneralInfo>& results);

    // only get first result
    void getResult(GeneralInfo& result);


private:
    Json::Value obj_;
    std::string url_;
    uint32_t result_num_;
    // file to save token key
    std::string filename_;
};

void generalTest();