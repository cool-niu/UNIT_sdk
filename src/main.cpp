#include "AccessToken.h"
#include "Utterance.h"
#include "VoiceRecognition.h"
#include "VoiceSynthesis.h"

#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h"

#include <string>
#include <iostream>
int main(int argc, char* argv[])
{
    std::string access;
    std::string ak = "IMwVFLLuNB9g74ZTOpw0c8v6";
    std::string sk = "QbehCt8y9BkQ00QlDNlA1ekVZMSwSWx3";
    get_access_token(access, ak, sk);


    std::string resBuf;
    VoiceRecognOnline(resBuf, access, "../voice/test.pcm");
    std::cout << resBuf << std::endl;

    std::cout << "可以开始进行对话：";
    std::string input;
    std::string session_id;
    while(std::cin >> input)
    {
        static int start = 0;
        if(input == "q")
        {
            break;
        }
        std::string result, request_body;
        rapidjson::Document doc;
        doc.SetObject();
        rapidjson::Document::AllocatorType &allocator = doc.GetAllocator();
        rapidjson::Value val(rapidjson::kStringType);
        val.SetString(input.c_str(), allocator);
        doc.AddMember("scene_id", 9970, allocator);
        doc.AddMember("query", val, allocator);
        if(!start)
        {
            doc.AddMember("session_id", "", allocator);            
        }
        else
        {
            rapidjson::Value val(rapidjson::kStringType);
            val.SetString(session_id.c_str(), allocator);
            doc.AddMember("session_id", val, allocator);            
        }

        rapidjson::StringBuffer buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        doc.Accept(writer);
        
        unit_utterance(result, buffer.GetString(), access);    
        // std::cout << result << std::endl;

        rapidjson::Document ret_doc;
        ret_doc.Parse(result.c_str());
        if(ret_doc.HasParseError())
        { 
            std::cout << "result parse error" << std::endl;
            return -1;
        }
        std::cout << result << std::endl;
        rapidjson::Value &ret = ret_doc["result"];
        
        rapidjson::Value &sess_id = ret["session_id"];
        assert(sess_id.IsString());
        session_id = sess_id.GetString();

        rapidjson::Value &actions = ret["action_list"];
        
        assert(actions.IsArray());
        for(rapidjson::SizeType i = 0; i < actions.Size(); ++i)
        {
            rapidjson::Value &v = actions[i];
            std::cout << v["say"].GetString() << std::endl;
            std::string text = v["say"].GetString();
            VoiceSynthesisOnline(access, text);
        }
        
    }
      
    return 0;
}