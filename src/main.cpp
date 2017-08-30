#include "AccessToken.h"
#include <string>
#include <iostream>
int main()
{
    std::string access;
    std::string ak = "IMwVFLLuNB9g74ZTOpw0c8v6";
    std::string sk = "QbehCt8y9BkQ00QlDNlA1ekVZMSwSWx3";
    get_access_token(access, ak, sk);
    std::cout << access << std::endl;
    return 0;
}