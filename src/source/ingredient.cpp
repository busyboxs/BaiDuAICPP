#include "Ingredient.h"



Ingredient::Ingredient()
{
    filename_ = "tokenKey";
    url_ = "https://aip.baidubce.com/rest/2.0/image-classify/v1/classify/ingredient";
}


Ingredient::~Ingredient()
{
}

Json::Value Ingredient::request(std::string imgBase64, std::map<std::string, std::string>& options)
{
    if (options.find("top_num") == options.end()) { // if top_num param is empty
        top_num_ = 5;
    }
    else {
        int top_num = stoi(options["top_num"]);
        top_num_ = top_num <= 0 ? 5 : (top_num >= 20 ? 20 : top_num);
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
    // check for other error code
    checkErrorWithExit(obj);

    return obj;
}

void Ingredient::getAllResult(std::vector<IngredientInfo>& results)
{
    int len = obj_["result"].size();
    results.reserve(len);
    IngredientInfo tmp;

    for (int i = 0; i < len; ++i) {
        tmp.name = UTF8ToGB(obj_["result"][i]["name"].asString().c_str());
        tmp.score = obj_["result"][i]["score"].asDouble();

        results.push_back(tmp);
    }
}

void Ingredient::getResult(IngredientInfo& result)
{
    result.name = UTF8ToGB(obj_["result"][0]["name"].asString().c_str());
    result.score = obj_["result"][0]["score"].asDouble();
}

void ingredientTest() {
    std::cout << "size: " << sizeof(IngredientInfo) << "\n";

    // read image and encode to base64
    std::string imgFile = "./images/vegetable.png";
    std::string imgBase64;
    imgToBase64(imgFile, imgBase64);

    // set options
    std::map<std::string, std::string> options;
    options["top_num"] = "10";


    Json::Value obj;
    Ingredient ingredientObj;
    obj = ingredientObj.request(imgBase64, options);

    IngredientInfo result;
    ingredientObj.getResult(result);
    result.print();

    std::vector<IngredientInfo> results;
    ingredientObj.getAllResult(results);

    for (auto& vec : results) {
        vec.print();
    }
}