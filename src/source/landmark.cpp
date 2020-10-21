#include "Landmark.h"



Landmark::Landmark()
{
    filename_ = "tokenKey";
    url_ = "https://aip.baidubce.com/rest/2.0/image-classify/v1/landmark";
}


Landmark::~Landmark()
{
}

Json::Value Landmark::request(std::string imgBase64, std::map<std::string, std::string>& options)
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

void Landmark::getResult(LandmarkInfo& result)
{
    result.landmark = UTF8ToGB(obj_["result"]["landmark"].asString().c_str());
}

void landmarkTest()
{
    std::cout << "size: " << sizeof(LandmarkInfo) << "\n";

    // read image and encode to base64
    std::string imgFile = "./images/landmark.png";
    std::string imgBase64;
    imgToBase64(imgFile, imgBase64);

    // set options
    std::map<std::string, std::string> options;

    Json::Value obj;
    Landmark landmarkObj;
    obj = landmarkObj.request(imgBase64, options);

    LandmarkInfo result;
    landmarkObj.getResult(result);
    result.print();
}