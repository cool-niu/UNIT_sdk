#include "AccessToken.h"
#include "Utterance.h"

#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h"

#include <string>
#include <iostream>
int main()
{
    std::string access;
    std::string ak = "IMwVFLLuNB9g74ZTOpw0c8v6";
    std::string sk = "QbehCt8y9BkQ00QlDNlA1ekVZMSwSWx3";
    get_access_token(access, ak, sk);
    std::cout << access << std::endl;

    std::string result, request_body;
    rapidjson::Document doc;
    doc.SetObject();
    rapidjson::Document::AllocatorType &allocator = doc.GetAllocator();
    doc.AddMember("scene_id", 9970, allocator);
    doc.AddMember("query", "切换下一件衣服", allocator);
    doc.AddMember("session_id", "", allocator);
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    doc.Accept(writer);
    
    unit_utterance(result, buffer.GetString(), access);

    std::cout << result << std::endl;
    return 0;
}