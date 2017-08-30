#ifndef _ACCESS_TOKEN_
#define _ACCESS_TOKEN_

#include <string>

static size_t callback(void *ptr, size_t size, size_t nmemb, void *stream);
int get_access_token(std::string &access_token, const std::string &AK, const std::string &SK);

#endif