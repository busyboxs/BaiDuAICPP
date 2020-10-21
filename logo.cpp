#include "Logo.h"

Logo::Logo()
{
    filename_ = "tokenKey";
    url_ = "https://aip.baidubce.com/rest/2.0/image-classify/v2/logo";
}


Logo::~Logo()
{
}

Json::Value Logo::request(std::string imgBase64, std::map<std::string, std::string>& options)
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

uint32_t Logo::getResultNum()
{
    return obj_["result_num"].asInt();
}

void Logo::getAllResult(std::vector<LogoInfo>& results)
{
    result_num_ = getResultNum();
    results.reserve(result_num_);
    LogoInfo tmp;

    for (uint32_t i = 0; i < result_num_; ++i) {
        tmp.type = obj_["result"][i]["type"].asInt();
        tmp.name = UTF8ToGB(obj_["result"][i]["name"].asString().c_str());
        tmp.probability = obj_["result"][i]["probability"].asFloat();
        tmp.location.left = obj_["result"][i]["location"]["left"].asInt();
        tmp.location.top = obj_["result"][i]["location"]["top"].asInt();
        tmp.location.width = obj_["result"][i]["location"]["width"].asInt();
        tmp.location.height = obj_["result"][i]["location"]["height"].asInt();
        results.push_back(tmp);
    }
}

void Logo::getResult(LogoInfo& result)
{
    result.type = obj_["result"][0]["type"].asInt();
    result.name = UTF8ToGB(obj_["result"][0]["name"].asString().c_str());
    result.probability = obj_["result"][0]["probability"].asFloat();
    result.location.left = obj_["result"][0]["location"]["left"].asInt();
    result.location.top = obj_["result"][0]["location"]["top"].asInt();
    result.location.width = obj_["result"][0]["location"]["width"].asInt();
    result.location.height = obj_["result"][0]["location"]["height"].asInt();
}

void logoTest()
{
    std::cout << "size: " << sizeof(LogoInfo) << "\n";

    // read image and encode to base64
    std::string imgFile = "./images/logo_test.jpg";
    std::string imgBase64;
    imgToBase64(imgFile, imgBase64);

    // set options
    std::map<std::string, std::string> options;
    // options["custom_lib"] = true;


    Json::Value obj;
    Logo logoObj;
    obj = logoObj.request(imgBase64, options);

    LogoInfo result;
    logoObj.getResult(result);
    result.print();
    cv::Mat img = cv::imread(imgFile);
    result.draw(img);
    cv::namedWindow("Logo Test", cv::WINDOW_NORMAL);
    cv::imshow("Logo Test", img);

    std::vector<LogoInfo> results;
    logoObj.getAllResult(results);

    cv::Mat img1 = cv::imread(imgFile);
    cv::namedWindow("Logo Tests", cv::WINDOW_NORMAL);

    for (auto& vec : results) {
        vec.print();
        vec.draw(img1);
    }
    cv::imshow("Logo Tests", img1);
    cv::waitKey();
}