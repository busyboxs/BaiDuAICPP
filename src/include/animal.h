#pragma once
#include <string>
#include <map>
#include <curl/curl.h>
#include <json/json.h>
#include <iostream>
#include <vector>
#include <cstdlib>
#include "util.h"

struct AnimalInfo {
    std::string name;
    std::string score;
    std::string baikeurl;
    std::string imageurl;
    std::string baikedesc;

    void print() {
        std::cout << std::setw(30) << std::setfill('-') << '\n';
        std::cout << "name: " << name << "\n";
        std::cout << "score: " << std::fixed << std::setprecision(4) << std::stof(score) << "\n";
        if (baikeurl != "null")
            std::cout << "baikeurl: " << baikeurl << "\n";
        if (imageurl != "null")
            std::cout << "imageurl: " << imageurl << "\n";
        if (baikedesc != "null")
            std::cout << "baikedesc: " << baikedesc << "\n";
    }
};

class Animal
{
public:
    Animal();
    ~Animal();

    Json::Value request(std::string imgBase64, std::map<std::string, std::string>& options);

    // get all return results
    void getAllResult(std::vector<AnimalInfo>& results);

    // only get first result
    void getResult(AnimalInfo& result);


private:
    Json::Value obj_;
    std::string url_;
    int top_num_;
    // file to save token key
    std::string filename_;
};

void animalTest();