#include "VoiceSynthesis.h"

#include "curl.h"
#include "easy.h"
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h"

#include <stdio.h>
#include <stdlib.h>

const int MAX_BODY_SIZE = 1024;

static size_t CallBack(void *ptr, size_t size, size_t nmemb, char **result)
{
    size_t result_len = size * nmemb;
    *result = (char *)realloc(*result, result_len + 1);
    if (*result == NULL)
    {
        printf("realloc failure!\n");
        return 1;
    }
    memcpy(*result, ptr, result_len);
    (*result)[result_len] = '\0';
    printf("%s\n", *result);
    return result_len;
}

int VoiceSynthesisOnline(const std::string& access, const std::string& voiceText)
{
    const char* cuid = "yzm_2017_niuxs";

    const std::string url = "http://tsn.baidu.com/text2audio";
    rapidjson::Document doc;
    doc.SetObject();
    rapidjson::Document::AllocatorType &allocator = doc.GetAllocator();
    rapidjson::Value val(rapidjson::kStringType);
    val.SetString(voiceText.c_str(), allocator);
    doc.AddMember("tex", val, allocator);
    doc.AddMember("lan", "zh", allocator);
    val.SetString(access.c_str(), allocator);
    doc.AddMember("tok", val, allocator);
    doc.AddMember("ctp", 1, allocator);
    val.SetString(cuid, allocator);
    val.AddMember("cuid", val, allocator);
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    doc.Accept(writer);

    CURL *curl;
    CURLcode res;
    char *resultBuf = NULL;
    struct curl_slist *headerlist = NULL;
    char tmp[1024];
    memset(tmp, 0, sizeof(tmp));

    snprintf(tmp, sizeof(tmp), "%s", "Content-Type: application/json; charset=utf-8");
    headerlist = curl_slist_append(headerlist, tmp);
    snprintf(tmp, sizeof(tmp), "Content-Length: %d", (int)buffer.GetLength());
    headerlist = curl_slist_append(headerlist, tmp);

    char body[MAX_BODY_SIZE] = {};
    memcpy(body, buffer.GetString(), (int)buffer.GetLength());
    curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_POST, 1);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, buffer.GetLength());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, CallBack);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &resultBuf);
    res = curl_easy_perform(curl);
    if (res != CURLE_OK)
    {
        printf("perform curl error:%d.\n", res);
        return -1;
    }

    printf("%s\n", resultBuf);

    return 0;
}
