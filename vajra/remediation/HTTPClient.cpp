#include <cstdlib>
#include "HTTPClient.h"

size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    size_t realsize = size * nmemb;
    auto *response = (std::string*)userp;
    response->append((const char*)contents, realsize);
    return realsize;
}

static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream)
{
    size_t written = fwrite(ptr, size, nmemb, (FILE *)stream);
    return written;
}

HTTPClient::HTTPClient()
{
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    response_code = -1;
}

HTTPClient::~HTTPClient()
{
    curl_easy_cleanup(curl);
    curl_global_cleanup();
}

bool HTTPClient::SendRequest(std::string request_url, RequestType type, std::string post_data)
{
    bool result = false;
    if (curl)
    {
        response = "";
        curl_easy_setopt(curl, CURLOPT_URL, request_url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&this->response);

        if (type == RequestType::POST)
        {
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_data.c_str());
            curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, post_data.length());
        }

        res = curl_easy_perform(curl);

        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);

        if (res == CURLE_OK)
            result = true;
        else
            result = false;
    }

    return result;
}

bool HTTPClient::DownloadFromUrl(std::string request_url, std::string path, RequestType type, std::string post_data)
{
    bool result = false;

    FILE *download_file = fopen(path.c_str(), "wb");

    if (curl && download_file)
    {
        response = "";
        curl_easy_setopt(curl, CURLOPT_URL, request_url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)download_file);
        curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);

        if (type == RequestType::POST)
        {
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_data.c_str());
            curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, post_data.length());
        }

        res = curl_easy_perform(curl);

        fclose(download_file);
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);

        if (res == CURLE_OK)
            result = true;
        else
            result = false;
    }

    return result;
}

std::string HTTPClient::GetResponse()
{
    return this->response;
}

long HTTPClient::GetHTTPResponseCode()
{
    return this->response_code;
}