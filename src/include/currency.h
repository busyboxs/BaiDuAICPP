#pragma once
#include "util.h"

struct CurrencyInfo {
    bool hasdetail;
    std::string currencyName;
    std::string currencyCode; // 货币代码
    std::string currencyDenomination; // 货币面值
    std::string year; // 货币年份

    void print() {
        std::cout << std::setw(30) << std::setfill('-') << '\n';
        std::cout << "currency name: " << currencyName << '\n';

        if (hasdetail) {
            std::cout << "currency code: " << currencyCode << '\n';
            std::cout << "currency denomination: " << currencyDenomination << '\n';
            std::cout << "year: " << year << '\n';
        }
    }
};

class Currency
{
public:
    Currency();
    ~Currency();

    Json::Value request(std::string imgBase64, std::map<std::string, std::string>& options);

    // only get first result
    void getResult(CurrencyInfo& result);


private:
    Json::Value obj_;
    std::string url_;
    // file to save token key
    std::string filename_;
};

void currencyTest();