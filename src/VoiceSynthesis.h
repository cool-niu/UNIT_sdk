#ifndef _VOICE_SYNTHESIS_H_
#define _VOICE_SYNTHESIS_H_

#include <string>

static size_t CallBack(void *ptr, size_t size, size_t nmemb, char **result);

int VoiceSynthesisOnline(const std::string& access, std::string& voiceText);


#endif