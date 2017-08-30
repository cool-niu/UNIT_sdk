#ifndef _UTTERANCE_
#define _UTTERANCE_

#include <string>

static size_t UtterCallBack(void *ptr, size_t size, size_t nmemb, void *stream);

int unit_utterance(std::string &json_result, const std::string json_request_body, const std::string &access_token);



#endif