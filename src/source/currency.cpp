#include "Currency.h"

Currency::Currency()
{
    filename_ = "tokenKey";
    url_ = "https://aip.baidubce.com/rest/2.0/image-classify/v1/currency";
}


Currency::~Currency()
{
}

Json::Value Currency::request(std::string imgBase64, std::map<std::string, std::string>& options)
{
    std::string response;
    Json::Value obj;
    std::string token;

    // 1. get HTTP post body
    std::string body;
    mergeHttpPostBody(body, imgBase64, options);

    // 2. get HTTP url with access token
    std::string url = url_;
    getHttpPostUrl(url, filename_, token);

    // 3. post request, response store the result
    int status_code = httpPostRequest(url, body, response);
    if (status_code != CURLcode::CURLE_OK) {
        obj["curl_error_code"] = status_code;
        obj_ = obj;
        return obj; // TODO: maybe should exit 
    }

    // 4. make string to json object
    generateJson(response, obj);

    // if access token is invalid or expired, we will get a new one
    if (obj["error_code"].asInt() == 110 || obj["error_code"].asInt() == 111) {
        token = getTokenKey();
        writeFile(filename_, token);
        return request(imgBase64, options);
    }

    obj_ = obj;
    // check for other error code
    checkErrorWithExit(obj);

    return obj;
}

void Currency::getResult(CurrencyInfo& result)
{
    result.hasdetail = obj_["result"]["hasdetail"].asBool();
    result.currencyName = UTF8ToGB(obj_["result"]["currencyName"].asString().c_str());
    result.currencyCode = obj_["result"]["currencyCode"].asString();
    result.currencyDenomination = UTF8ToGB(obj_["result"]["currencyDenomination"].asString().c_str());
    result.year = UTF8ToGB(obj_["result"]["year"].asString().c_str());
}

void currencyTest()
{
    std::cout << "size: " << sizeof(CurrencyInfo) << "\n";

    // read image and encode to base64
    std::string imgFile = "./images/dollar.png";
    std::string imgBase64;
    imgToBase64(imgFile, imgBase64);

    // set options
    std::map<std::string, std::string> options;

    Json::Value obj;
    Currency currencyObj;
    obj = currencyObj.request(imgBase64, options);

    CurrencyInfo result;
    currencyObj.getResult(result);
    result.print();
}