#include "Redwine.h"



Redwine::Redwine()
{
    filename_ = "tokenKey";
    url_ = "https://aip.baidubce.com/rest/2.0/image-classify/v1/redwine";
}


Redwine::~Redwine()
{
}

Json::Value Redwine::request(std::string imgBase64, std::map<std::string, std::string>& options)
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

void Redwine::getResult(RedwineInfo& result)
{
    result.hasdetail = obj_["result"]["hasdetail"].asBool();
    result.wineNameCn = UTF8ToGB(obj_["result"]["wineNameCn"].asString().c_str());
    result.wineNameEn = obj_["result"]["wineNameEn"].asString();
    result.countryCn = UTF8ToGB(obj_["result"]["countryCn"].asString().c_str());
    result.countryEn = obj_["result"]["countryEn"].asString();
    result.regionCn = UTF8ToGB(obj_["result"]["regionCn"].asString().c_str());
    result.regionEn = obj_["result"]["regionEn"].asString();
    result.subRegionCn = UTF8ToGB(obj_["result"]["subRegionCn"].asString().c_str());
    result.subRegionEn = obj_["result"]["subRegionEn"].asString();
    result.wineryCn = UTF8ToGB(obj_["result"]["wineryCn"].asString().c_str());
    result.wineryEn = obj_["result"]["wineryEn"].asString();
    result.classifyByColor = UTF8ToGB(obj_["result"]["classifyByColor"].asString().c_str());
    result.classifyBySugar = UTF8ToGB(obj_["result"]["classifyBySugar"].asString().c_str());
    result.color = UTF8ToGB(obj_["result"]["color"].asString().c_str());
    result.grapeCn = UTF8ToGB(obj_["result"]["grapeCn"].asString().c_str());
    result.grapeEn = UTF8ToGB(obj_["result"]["grapeEn"].asString().c_str());
    result.tasteTemperature = UTF8ToGB(obj_["result"]["tasteTemperature"].asString().c_str());
    result.description = UTF8ToGB(obj_["result"]["description"].asString().c_str());
}

void redwineTest()
{
    std::cout << "size: " << sizeof(RedwineInfo) << "\n";

    // read image and encode to base64
    std::string imgFile = "./images/wine.png";
    std::string imgBase64;
    imgToBase64(imgFile, imgBase64);

    // set options
    std::map<std::string, std::string> options;

    Json::Value obj;
    Redwine redwineObj;
    obj = redwineObj.request(imgBase64, options);

    RedwineInfo result;
    redwineObj.getResult(result);
    result.print();

}