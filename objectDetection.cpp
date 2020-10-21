#include "ObjectDetection.h"

ObjectDetection::ObjectDetection()
{
    filename_ = "tokenKey";
    url_ = "https://aip.baidubce.com/rest/2.0/image-classify/v1/object_detect";
}


ObjectDetection::~ObjectDetection()
{
}

Json::Value ObjectDetection::request(std::string imgBase64, std::map<std::string, std::string>& options)
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

    return obj;
}

void ObjectDetection::getResult(ObjDetInfo& result)
{
    result.left = obj_["result"].get("left", "0").asInt();
    result.top = obj_["result"].get("top", "0").asInt();
    result.width = obj_["result"].get("width", "0").asInt();
    result.height = obj_["result"].get("height", "0").asInt();
}

void objectDetectionTest()
{
    std::cout << "size: " << sizeof(ObjDetInfo) << "\n";

    // read image and encode to base64
    std::string img_file = "./images/cat.jpg";
    std::string out;
    readImageFile(img_file.c_str(), out);
    std::string img_base64 = base64_encode(out.c_str(), (int)out.size());

    // set options
    std::map<std::string, std::string> options;
    options["with_face"] = "0";


    Json::Value obj;
    ObjectDetection objDetObj;
    obj = objDetObj.request(img_base64, options);
    //std::cout << (obj.get("result", "null")) << std::endl;
    ObjDetInfo result;
    objDetObj.getResult(result);
    result.print();

    cv::Mat img = cv::imread(img_file);
    result.draw(img);
    cv::namedWindow("Object Detection", cv::WINDOW_NORMAL);
    cv::imshow("Object Detection", img);
    cv::waitKey();
}