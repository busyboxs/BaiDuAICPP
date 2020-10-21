#include "Plant.h"



Plant::Plant()
{
    filename_ = "tokenKey";
    url_ = "https://aip.baidubce.com/rest/2.0/image-classify/v1/plant";
}


Plant::~Plant()
{
}

Json::Value Plant::request(std::string imgBase64, std::map<std::string, std::string>& options)
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

void Plant::getAllResult(std::vector<PlantInfo>& results)
{
    int len = obj_["result"].size();
    results.reserve(len);
    PlantInfo tmp;

    for (int i = 0; i < len; ++i) {
        tmp.name = UTF8ToGB(obj_["result"][i]["name"].asString().c_str());
        tmp.score = obj_["result"][i]["score"].asFloat();
        tmp.baikeurl = obj_["result"][i]["baike_info"].get("baike_url", "null").asString();
        tmp.imageurl = obj_["result"][i]["baike_info"].get("image_url", "null").asString();
        tmp.baikedesc = UTF8ToGB(obj_["result"][i]["baike_info"].get("description", "null").asString().c_str());
        results.push_back(tmp);
    }
}

void Plant::getResult(PlantInfo& result)
{
    result.name = UTF8ToGB(obj_["result"][0]["name"].asString().c_str());
    result.score = obj_["result"][0]["score"].asFloat();
    result.baikeurl = obj_["result"][0]["baike_info"].get("baike_url", "null").asString();
    result.imageurl = obj_["result"][0]["baike_info"].get("image_url", "none").asString();
    result.baikedesc = UTF8ToGB(obj_["result"][0]["baike_info"].get("description", "none").asString().c_str());
}

void plantTest()
{

    std::cout << "size: " << sizeof(PlantInfo) << "\n";

    // read image and encode to base64
    std::string imgFile = "./images/plant_test.jpg";
    std::string imgBase64;
    imgToBase64(imgFile, imgBase64);

    // set options
    std::map<std::string, std::string> options;
    options["baike_num"] = "5";


    Json::Value obj;
    Plant plantObj;
    obj = plantObj.request(imgBase64, options);

    PlantInfo result;
    plantObj.getResult(result);
    result.print();

    std::vector<PlantInfo> results;
    plantObj.getAllResult(results);

    for (auto& vec : results) {
        vec.print();
    }
}