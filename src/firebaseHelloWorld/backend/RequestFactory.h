#ifndef REQUESTFACTORY_H
#define REQUESTFACTORY_H

#include "Request.h"
#include <json/json.h>

class RequestFactory
{
public:
	static RequestsVector CreateRequests(const std::string& document);
private:
	static IRequestPtrS CreateRequest(const Json::Value& key, const Json::Value& value);
};

#endif // REQUESTFACTORY_H