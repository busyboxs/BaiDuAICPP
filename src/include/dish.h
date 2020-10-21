#pragma once
#include "util.h"

struct DishInfo {
    std::string name;
    bool has_calorie;
    float calorie;
    float probability;
    std::string baikeurl;
    std::string imageurl;
    std::string baikedesc;

    void print() {
        std::cout << std::setw(30) << std::setfill('-') << '\n';
        std::cout << "name: " << name << "\n";

        if (has_calorie)
            std::cout << "calorie: " << calorie << "J" << "\n";
        else
            std::cout << "has_calorie: false\n";

        std::cout << "probability: " << probability << "\n";

        if (baikeurl != "null")
            std::cout << "baikeurl: " << baikeurl << "\n";
        if (imageurl != "null")
            std::cout << "imageurl: " << imageurl << "\n";
        if (baikedesc != "null")
            std::cout << "baikedesc: " << baikedesc << "\n";
    }
};

class Dish
{
public:
    Dish();
    ~Dish();
    Json::Value request(std::string imgBase64, std::map<std::string, std::string>& options);

    // get all return results
    void getAllResult(std::vector<DishInfo>& results);

    // only get first result
    void getResult(DishInfo& result);


private:
    Json::Value obj_;
    std::string url_;
    // file to save token key
    std::string filename_;
};

void dishTest();