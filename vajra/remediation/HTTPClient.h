#ifndef FLEET_HTTPCLIENT_H
#define FLEET_HTTPCLIENT_H

#include <string>
#include <curl/curl.h>

enum RequestType
{
    GET,
    POST
};

class HTTPClient
{
private:
    CURL *curl;
    CURLcode res;
    std::string response;
    long response_code;
public:
    HTTPClient();
    ~HTTPClient();
    bool SendRequest(std::string request_url, RequestType type = RequestType::GET, std::string post_data = "");
    bool DownloadFromUrl(std::string request_url, std::string path, RequestType type = RequestType::GET, std::string post_data = "");
    std::string GetResponse();
    long GetHTTPResponseCode();
};


#endif //FLEET_HTTPCLIENT_H
