#ifndef _VOICE_RECOGNITION_H_
#define _VOICE_RECOGNITION_H_

#include <string>

static size_t writefunc(void *ptr, size_t size, size_t nmemb, char **result);
int VoiceRecognOnline(std::string &result, const std::string& access, const char* voiceFile);



#endif