#include "General.h"

General::General()
{
    filename_ = "tokenKey";
    url_ = "https://aip.baidubce.com/rest/2.0/image-classify/v2/advanced_general";
}


General::~General()
{
}

Json::Value General::request(std::string imgBase64, std::map<std::string, std::string>& options)
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

    checkErrorWithExit(obj);

    result_num_ = getResultNum();

    return obj;
}

int General::getResultNum()
{
    return obj_["result_num"].asInt();
}

void General::getAllResult(std::vector<GeneralInfo>& results)
{
    results.reserve(result_num_);
    GeneralInfo tmp;

    for (uint32_t i = 0; i < result_num_; ++i) {
        tmp.keyword = UTF8ToGB(obj_["result"][i]["keyword"].asString().c_str());
        tmp.score = obj_["result"][i]["score"].asFloat();
        tmp.root = UTF8ToGB(obj_["result"][i]["root"].asString().c_str());
        tmp.baikeurl = obj_["result"][i]["baike_info"].get("baike_url", "null").asString();
        tmp.imageurl = obj_["result"][i]["baike_info"].get("image_url", "null").asString();
        tmp.baikedesc = UTF8ToGB(obj_["result"][i]["baike_info"].get("description", "null").asString().c_str());
        results.push_back(tmp);
    }
}

void General::getResult(GeneralInfo& result)
{
    result.keyword = UTF8ToGB(obj_["result"][0]["keyword"].asString().c_str());
    result.score = obj_["result"][0]["score"].asFloat();
    result.root = UTF8ToGB(obj_["result"][0]["root"].asString().c_str());
    result.baikeurl = obj_["result"][0]["baike_info"].get("baike_url", "null").asString();
    result.imageurl = obj_["result"][0]["baike_info"].get("image_url", "null").asString();
    result.baikedesc = UTF8ToGB(obj_["result"][0]["baike_info"].get("description", "null").asString().c_str());
}


void generalTest()
{

    std::cout << "size: " << sizeof(GeneralInfo) << "\n";

    // read image and encode to base64
    std::string imgFile = "./images/001.png";
    std::string imgBase64;
    imgToBase64(imgFile, imgBase64);

    // set options
    std::map<std::string, std::string> options;
    options["baike_num"] = "5";


    Json::Value obj;
    General generalObj;
    obj = generalObj.request(imgBase64, options);

    GeneralInfo result;
    generalObj.getResult(result);
    result.print();

    std::vector<GeneralInfo> results;
    generalObj.getAllResult(results);

    for (auto& vec : results) {
        vec.print();
    }
}