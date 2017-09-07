#include "VoiceRecognition.h"
#include "base64.h"

#include "curl.h"
#include "easy.h"
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h"

#include <stdio.h>
#include <stdlib.h>

#define _METHOD_1_ 1
#define _METHOD_2_ 0

#define MAX_BUFFER_SIZE 1024
#define MAX_BODY_SIZE 1000000

static size_t writefunc(void *ptr, size_t size, size_t nmemb, char **result)
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



int VoiceRecognOnline(std::string &result, const std::string& access, const char* voiceFile)
{
    FILE *fp = NULL;
    fp = fopen(voiceFile, "r");
    if (NULL == fp)
    {
        return -1;
    }
    fseek(fp, 0, SEEK_END);
    int content_len = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    char *audiodata = (char *)malloc(content_len);
    fread(audiodata, content_len, sizeof(char), fp);

    //put your own params here
    const char *cuid = "yzm_2017_niuxs";
    const char *apiKey = "IMwVFLLuNB9g74ZTOpw0c8v6";
    const char *secretKey = "QbehCt8y9BkQ00QlDNlA1ekVZMSwSWx3";

    std::string token = access;
    char host[MAX_BUFFER_SIZE];
    // snprintf(host, sizeof(host), 
    //         "https://openapi.baidu.com/oauth/2.0/token?grant_type=client_credentials&client_id=%s&client_secret=%s", 
    //         apiKey, secretKey);
    // FILE* fpp = NULL;
    // char cmd[MAX_BUFFER_SIZE];
    // char* result = (char*)malloc(MAX_BUFFER_SIZE);
    // const char* curl_cmd = "curl -s ";
    // const char* yinhao = "\"";
    // strcpy(cmd, curl_cmd);
    // strcat(cmd, yinhao);
    // strcat(cmd, host);
    // strcat(cmd, yinhao);
    // fpp = popen(cmd, "r");
    // fgets(result, MAX_BUFFER_SIZE, fpp);
    // pclose(fpp);

    // if (result != NULL) 
    // {
    //     printf("%s\n", result);
    //     rapidjson::Document doc;
    //     doc.Parse(result);
    //     assert(!doc.HasParseError());
    //     token = doc["access_token"].GetString();
    // }

    memset(host, 0, sizeof(host));
    snprintf(host, sizeof(host), "%s", "http://vop.baidu.com/server_api");

#if _METHOD_1_
    //method 1
    char tmp[MAX_BUFFER_SIZE];
    memset(tmp, 0, sizeof(tmp));
    char body[MAX_BODY_SIZE];
    memset(body, 0, sizeof(body));
    std::string decode_data = base64_encode((const unsigned char *)audiodata, content_len);
    if (0 == decode_data.length())
    {
        printf("base64 encoded data is empty.\n");
        return 1;
    }
    rapidjson::Document doc;
    doc.SetObject();
    rapidjson::Document::AllocatorType & allocator = doc.GetAllocator();
    doc.AddMember("format", "pcm", allocator);
    doc.AddMember("rate", 8000, allocator);
    doc.AddMember("channel", 1, allocator);
    rapidjson::Value val(rapidjson::kStringType);
    val.SetString(token.c_str(), allocator);
    doc.AddMember("token",  val, allocator);

    val.SetString(cuid, allocator);    
    doc.AddMember("cuid", val, allocator);
    val.SetString(decode_data.c_str(), allocator);
    doc.AddMember("speech", val, allocator);
    doc.AddMember("len", content_len, allocator);

    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    doc.Accept(writer);

//    buffer["url"]  = url;
//    buffer["callback"]     = callback;

    content_len = buffer.GetLength();
    std::string buf = buffer.GetString();
    memcpy(body, buf.c_str(), content_len);

    CURL *curl;
    CURLcode res;
    char *resultBuf = NULL;
    struct curl_slist *headerlist = NULL;
    snprintf(tmp, sizeof(tmp), "%s", "Content-Type: application/json; charset=utf-8");
    headerlist = curl_slist_append(headerlist, tmp);
    snprintf(tmp, sizeof(tmp), "Content-Length: %d", content_len);
    headerlist = curl_slist_append(headerlist, tmp);

    curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, host);
    curl_easy_setopt(curl, CURLOPT_POST, 1);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, content_len);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &resultBuf);
    res = curl_easy_perform(curl);
    if (res != CURLE_OK)
    {
        printf("perform curl error:%d.\n", res);
        return 1;
    }
    
    rapidjson::Document res_doc;
    res_doc.Parse(resultBuf);
    assert(!res_doc.HasParseError());
    rapidjson::Value array_res;
    array_res = res_doc["result"];
    assert(array_res.IsArray());
    result = array_res[0].GetString();

    curl_slist_free_all(headerlist);
    curl_easy_cleanup(curl);

#else
    //second way, post raw data
    char tmp[MAX_BUFFER_SIZE];
    memset(tmp, 0, sizeof(tmp));
    snprintf(tmp, sizeof(tmp), "?cuid=%s&token=%s", cuid, token.c_str());
    strcat(host, tmp);

    CURL *curl;
    CURLcode res;
    char *resultBuf = NULL;
    struct curl_slist *headerlist = NULL;
    snprintf(tmp, sizeof(tmp), "%s","Content-Type: audio/pcm; rate=8000");
    headerlist = curl_slist_append(headerlist, tmp);
    snprintf(tmp, sizeof(tmp), "Content-Length: %d", content_len);
    headerlist = curl_slist_append(headerlist, tmp);

    curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, host);
    curl_easy_setopt(curl, CURLOPT_POST, 1);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30); 
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, audiodata);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, content_len);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &resultBuf);
    res = curl_easy_perform(curl);
    if (res != CURLE_OK)
    {
        printf("perform curl error:%d.\n", res);
        return 1;
    }

    rapidjson::Document res_doc;
    res_doc.Parse(resultBuf);
    assert(!res_doc.HasParseError());
    rapidjson::Value array_res;
    array_res = res_doc["result"];
    assert(array_res.IsArray());
    result = array_res[0].GetString();

    curl_slist_free_all(headerlist);
    curl_easy_cleanup(curl);

#endif

    fclose(fp);
    free(audiodata);
    return 0;
    
}
