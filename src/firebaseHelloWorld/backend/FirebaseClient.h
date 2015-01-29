#ifndef FIREBASECLIENT_H
#define FIREBASECLIENT_H

#include <string>

class FirebaseClient
{
public:
    FirebaseClient(const std::string& baseUrl);
    ~FirebaseClient();

    std::string GetRequestsJsonString();
    void SetState(const std::string& status,
                  const std::int32_t targetTemp,
                  const std::string& targetTime,
                  const std::int32_t temp);
    void RemoveRequestFromList(const std::string& id);
    void AddResponseToServer(const std::string& id, const std::string& data);
private:
    // Unimplemented to avoid misusing
    FirebaseClient();

    struct MemoryStruct
    {
        char*  pMemory;
        size_t size;

        MemoryStruct()
        : pMemory(static_cast<char*>(malloc(1)))
        , size(0)
        {
        }

        ~MemoryStruct()
        {
            if(pMemory) free(pMemory);
        }
    };

    static size_t WriteMemoryCallback(void* contents, size_t size, size_t nmemb, void* userp);
    const char* GetMsgPoolUrl() const;
    const char* GetStateUrl() const;
    const char* GetRequestIdUrl(const std::string& id) const;
    const char* GetResponseUrl(const std::string& id) const;
    const std::string EncodeJson(const std::string& status,
                                 const std::int32_t targetTemp,
                                 const std::string& targetTime,
                                 const std::int32_t temp) const;

private:
    std::string mBaseUrl;
};

#endif // FIREBASECLIENT_H