#pragma once
#include "util.h"

struct IngredientInfo {
    std::string name;
    double score;

    void print() {
        std::cout << std::setw(30) << std::setfill('-') << '\n';
        std::cout << "name: " << name << '\n';
        std::cout << "score: " << score << '\n';
    }
};

class Ingredient
{
public:
    Ingredient();
    ~Ingredient();

    Json::Value request(std::string imgBase64, std::map<std::string, std::string>& options);

    // get all return results
    void getAllResult(std::vector<IngredientInfo>& results);

    // only get first result
    void getResult(IngredientInfo& result);


private:
    Json::Value obj_;
    std::string url_;
    uint32_t top_num_;
    // file to save token key
    std::string filename_;
};

void ingredientTest();