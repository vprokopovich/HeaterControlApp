#include "RequestFactory.h"
#include "Exceptions.h"
#include <CTrace.h>

RequestsVector RequestFactory::CreateRequests(const std::string& document)
{
	RequestsVector retVal;
	Json::Value  requests;
	Json::Reader reader;

	bool parsingSuccessful = reader.parse(document, requests);
	if (false == parsingSuccessful)
	{
	    throw JsonParsingException();
	}

	for (auto it = requests.begin(); it != requests.end(); it++)
	{
		Json::Value key = it.key();
		Json::Value value = (*it);

		auto request = CreateRequest(key, value);
		if (nullptr != request)
		{
			retVal.push_back(request);
		}
    }

    return retVal;
}

IRequestPtrS RequestFactory::CreateRequest(const Json::Value& key, const Json::Value& value)
{
	IRequestPtrS retVal;

	do
	{
		if (false == value["action"].isString()) break;
		if (false == key.isString()) break;

		if ("RequestSetTemp" == value["action"].asString())
		{
			if (false == value["value"].isInt()) break;

			return std::make_shared<RequestSetTemperature>(key.asString(), value["value"].asInt());
		}
		else if ("RequestDummy" == value["action"].asString())
		{
			return std::make_shared<RequestDummy>(key.asString());
		}
	} while(false);

	return retVal;
}