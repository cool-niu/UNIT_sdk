#include "VoiceSynthesis.h"

#include "curl.h"
#include "easy.h"
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h"
#include <iostream>
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

    std::string url = "http://tsn.baidu.com/text2audio";
    url = url + "?tex=" + voiceText;
    url = url + "&lan=zh";
    url = url + "&cuid=yzm_2017_niuxs";
    url = url + "&ctp=1";
    url = url + "&tok=" + access;
    // std::string url = "http://www.baidu.com";

    std::cout << url << std::endl;
    FILE *fp;
    if ((fp = fopen("get.mp3", "w")) == NULL)  // 返回结果用文件存储
        return false;

    CURL *curl;
    CURLcode res;
    char *resultBuf = NULL;
    struct curl_slist *headerlist = NULL;
    headerlist = curl_slist_append(headerlist, "Accept: Agent-007");

    curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
    curl_easy_setopt(curl, CURLOPT_HEADERDATA, fp);
    res = curl_easy_perform(curl);
    if (res != CURLE_OK)
    {
        
        printf("perform curl error:%d.\n", res);
        curl_slist_free_all(headerlist);
        curl_easy_cleanup(curl);
        return -1;
    }

    // printf("%s\n", resultBuf);
    fclose(fp);
    curl_slist_free_all(headerlist);
    curl_easy_cleanup(curl);
    return 0;
}
