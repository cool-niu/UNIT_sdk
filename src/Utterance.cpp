
#include "Utterance.h"

#include "curl.h"
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h"

#include <iostream>

// unit对话接口url
const static std::string get_utterance_url = 
         "https://aip.baidubce.com/rpc/2.0/solution/v1/unit_utterance";

static std::string s_get_utterance_result;

/**
 * curl发送http请求调用的回调函数，回调函数中对返回的json格式的body进行了解析，解析结果储存在全局的静态变量当中
 * @param 参数定义见libcurl文档
 * @return 返回值定义见libcurl文档
 */

static size_t UtterCallBack(void *ptr, size_t size, size_t nmemb, void *stream) {
    // 获取到的body存放在ptr中，先将其转换为string格式
    s_get_utterance_result = std::string((char *) ptr, size * nmemb);
    return size * nmemb;
}

/**
 * 调用对话接口，返回int格式的结果，具体格式解析见百度大脑文档
 * @param json_result 以string格式返回的json格式的结果
 * @param json_request_body 以string格式传递的json数据（如："{\"scene_id\":123,\"query\":\"今天北京天气怎么样?\", \"session_id\":\"\"}"; // json格式 ）

 * @param access_token 以string格式传入的access token数据，access token获取方式见access_token获取相关文档及代码
 * @return 调用成功返回0，发生错误返回其他错误码
 */
int unit_utterance(std::string &json_result, const std::string json_request_body,
             const std::string &access_token) {
    std::string url = get_utterance_url + "?access_token=" + access_token;
    CURL *curl = NULL;
    CURLcode result_code;
    int is_success = 0;
    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.data());
        curl_easy_setopt(curl, CURLOPT_POST, 1);
        curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Content-Type:application/json;charset=UTF-8");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_request_body.data());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, UtterCallBack);
        result_code = curl_easy_perform(curl);
        if (result_code != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(result_code));
            is_success = 1;
            return is_success;
        }
        json_result = s_get_utterance_result;
        curl_easy_cleanup(curl);
        is_success = 0;
    } else {
        fprintf(stderr, "curl_easy_init() failed.");
        is_success = 1;
    }
    return is_success;
}
