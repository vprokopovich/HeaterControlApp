#include "FirebaseClient.h"
#include <CTrace.h>
#include <cstring>
#include "Exceptions.h"
#include <curl/curl.h>
#include <json/json.h>

FirebaseClient::FirebaseClient(const std::string& baseUrl)
: mBaseUrl(baseUrl)
{
    curl_global_init(CURL_GLOBAL_ALL);
}

FirebaseClient::~FirebaseClient()
{
    curl_global_cleanup();
}

std::string FirebaseClient::GetRequestsJsonString()
{
    std::string retVal;
    CURLcode res;
    
    CURL* curl = curl_easy_init();
    if (curl)
    {
        MemoryStruct chunk;

        curl_easy_setopt(curl, CURLOPT_URL, GetMsgPoolUrl());
        curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, FirebaseClient::WriteMemoryCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

        res = curl_easy_perform(curl);

        if (res != CURLE_OK)
        {
            throw CurlException(curl_easy_strerror(res));
        }
        else
        {
            TRC_DEBUG("%lu bytes retrieved", static_cast<size_t>(chunk.size));
            retVal = std::string(chunk.pMemory, chunk.size);
        }

        curl_easy_cleanup(curl);
    }

    return retVal;
}

void FirebaseClient::SetState(const std::string& status,
                              const std::int32_t targetTemp,
                              const std::string& targetTime,
                              const std::int32_t temp)
{
    CURL* curl = curl_easy_init();
    CURLcode res;

    if (curl)
    {
        struct curl_slist* headers = NULL;

        std::string content = EncodeJson(status, targetTemp, targetTime, temp);
        std::string headerContentLength = "Content-Length: " + content.length();
        headers = curl_slist_append(headers, headerContentLength.c_str());

        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers); 
        curl_easy_setopt(curl, CURLOPT_URL, GetStateUrl());  
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");

        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, content.c_str());

        res = curl_easy_perform(curl);

        if (res != CURLE_OK)
        {
            throw CurlException(curl_easy_strerror(res));
        }

        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }
}

void FirebaseClient::RemoveRequestFromList(const std::string& id)
{
    CURL* curl = curl_easy_init();
    CURLcode res;

    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, GetRequestIdUrl(id));  
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");

        res = curl_easy_perform(curl);

        if (res != CURLE_OK)
        {
            throw CurlException(curl_easy_strerror(res));
        }

        curl_easy_cleanup(curl);
    }
}

void FirebaseClient::AddResponseToServer(const std::string& id, const std::string& data)
{
    CURL* curl = curl_easy_init();
    CURLcode res;

    if (curl)
    {
        struct curl_slist* headers = NULL;

        auto checkedData = data;
        if (checkedData.length() == 0)
        {
            checkedData = std::string("{}");
        }
        std::string headerContentLength = "Content-Length: " + checkedData.length();
        headers = curl_slist_append(headers, headerContentLength.c_str());

        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers); 
        curl_easy_setopt(curl, CURLOPT_URL, GetResponseUrl(id));  
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");

        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, checkedData.c_str());

        res = curl_easy_perform(curl);

        if (res != CURLE_OK)
        {
            throw CurlException(curl_easy_strerror(res));
        }

        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }
}

size_t FirebaseClient::WriteMemoryCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
    size_t realsize = size * nmemb;
    struct MemoryStruct* pMem = (struct MemoryStruct *)userp;

    pMem->pMemory = static_cast<char*>(realloc(pMem->pMemory, pMem->size + realsize + 1));
    if(pMem->pMemory == NULL)
    {
        /* out of memory! */ 
        printf("not enough memory (realloc returned NULL)\n");
        return 0;
    }
    memcpy(&(pMem->pMemory[pMem->size]), contents, realsize);
    pMem->size += realsize;
    pMem->pMemory[pMem->size] = 0;
    return realsize;
}

const char* FirebaseClient::GetMsgPoolUrl() const
{
    std::string url = mBaseUrl + "msg-pool.json";
    return url.c_str();
}

const char* FirebaseClient::GetStateUrl() const
{
    std::string url = mBaseUrl + "state.json";
    return url.c_str();
}

const char* FirebaseClient::GetRequestIdUrl(const std::string& id) const
{
    std::string url = mBaseUrl + "msg-pool/" + id + ".json";
    return url.c_str();
}

const char* FirebaseClient::GetResponseUrl(const std::string& id) const
{
    std::string url = mBaseUrl + "responses/" + id + ".json";
    return url.c_str();   
}

const std::string FirebaseClient::EncodeJson(const std::string& status,
                                             const std::int32_t targetTemp,
                                             const std::string& targetTime,
                                             const std::int32_t temp) const
{
    Json::Value root;
    root["status"] = status;
    root["targetTemp"] = targetTemp;
    root["targetTime"] = targetTime;
    root["temp"] = temp;

    Json::FastWriter writer;
    std::string retVal = writer.write(root);

    return retVal.c_str();
}