#pragma once
#include "util.h"

struct RedwineInfo {
    bool hasdetail;
    std::string wineNameCn; // 红酒中文名
    std::string wineNameEn;
    std::string countryCn;
    std::string countryEn;
    std::string regionCn;
    std::string regionEn;
    std::string subRegionCn;
    std::string subRegionEn;
    std::string wineryCn; // 酒庄
    std::string wineryEn;
    std::string classifyByColor; // 类型
    std::string classifyBySugar; // 糖分类型
    std::string color; // 色泽
    std::string grapeCn; // 品种
    std::string grapeEn;
    std::string tasteTemperature; // 品尝温度
    std::string description;

    void print() {
        std::cout << std::setw(30) << std::setfill('-') << '\n';
        std::cout << "name: " << wineNameCn << " - " << wineNameEn << '\n';

        if (hasdetail) {
            std::cout << "country: " << countryCn << " - " << countryEn << '\n';
            std::cout << "region: " << countryCn << " - " << regionEn << '\n';
            std::cout << "subregion: " << subRegionCn << " - " << subRegionEn << '\n';
            std::cout << "winery: " << wineryCn << " - " << wineryEn << '\n';
            std::cout << "classify by color: " << classifyByColor << '\n';
            std::cout << "classify by sugar: " << classifyBySugar << '\n';
            std::cout << "color: " << color << '\n';
            std::cout << "grape: " << grapeCn << " - " << grapeEn << '\n';
            std::cout << "taste temperature: " << tasteTemperature << '\n';
            std::cout << "description: " << description << '\n';
        }

    }
};

class Redwine
{
public:
    Redwine();
    ~Redwine();

    Json::Value request(std::string imgBase64, std::map<std::string, std::string>& options);

    // only get first result
    void getResult(RedwineInfo& result);


private:
    Json::Value obj_;
    std::string url_;
    // file to save token key
    std::string filename_;
};

void redwineTest();