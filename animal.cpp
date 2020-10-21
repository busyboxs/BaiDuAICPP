#include "animal.h"

Animal::Animal()
{
    url_ = "https://aip.baidubce.com/rest/2.0/image-classify/v1/animal";
    top_num_ = 6; // default value
}


Animal::~Animal()
{
}

Json::Value Animal::request(std::string imgBase64, std::map<std::string, std::string>& options)
{
    if (!options["top_num"].empty()) {
        top_num_ = std::stoi(options["top_num"]);
    }

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

    return obj;
}

void Animal::getAllResult(std::vector<AnimalInfo>& results)
{
    results.reserve(top_num_);
    AnimalInfo tmp;

    for (int i = 0; i < top_num_; ++i) {
        tmp.name = UTF8ToGB(obj_["result"][i]["name"].asString().c_str());
        tmp.score = obj_["result"][i]["score"].asString();
        tmp.baikeurl = obj_["result"][i]["baike_info"].get("baike_url", "null").asString();
        tmp.imageurl = obj_["result"][i]["baike_info"].get("image_url", "null").asString();
        tmp.baikedesc = UTF8ToGB(obj_["result"][i]["baike_info"].get("description", "null").asString().c_str());
        results.push_back(tmp);
    }
}

void Animal::getResult(AnimalInfo& result)
{
    result.name = UTF8ToGB(obj_["result"][0]["name"].asString().c_str());
    result.score = obj_["result"][0]["score"].asString();
    result.baikeurl = obj_["result"][0]["baike_info"].get("baike_url", "null").asString();
    result.imageurl = obj_["result"][0]["baike_info"].get("image_url", "none").asString();
    result.baikedesc = UTF8ToGB(obj_["result"][0]["baike_info"].get("description", "none").asString().c_str());
}


void animalTest() {
    // read image and encode to base64
    std::string img_file = "./images/cat.jpg";
    std::string out;
    readImageFile(img_file.c_str(), out);
    std::string img_base64 = base64_encode(out.c_str(), (int)out.size());

    // set options
    std::map<std::string, std::string> options;
    options["top_num"] = "5";
    options["baike_num"] = "5";


    Json::Value obj;
    Animal animalObj;
    AnimalInfo result;
    obj = animalObj.request(img_base64, options);

    animalObj.getResult(result);
    result.print();

    std::vector<AnimalInfo> results;
    animalObj.getAllResult(results);

    for (auto& vec : results) {
        vec.print();
    }
}