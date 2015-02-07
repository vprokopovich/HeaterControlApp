#include "Request.h"
#include <CTrace.h>
#include "FirebaseClient.h"
#include <json/json.h>

RequestSetTemperature::RequestSetTemperature(const std::string& id, const std::uint32_t temperature)
: mId(id)
, mTemperature(temperature)
{
}

RequestSetTemperature::~RequestSetTemperature()
{
}

void RequestSetTemperature::Process()
{
	FirebaseClient client("https://heater-control.firebaseio.com/");
	client.SetState("Done", mTemperature, "currentTime", mTemperature + 2);
	client.RemoveRequestFromList(mId);

    Json::Value jsonResponse;
    jsonResponse["status"] = std::string("Heating");
    jsonResponse["targetTemperature"] = mTemperature;
    jsonResponse["targetTime"] = std::string("CurrentTime");
    jsonResponse["currentTemperature"] = mTemperature + 2;

    Json::FastWriter writer;
    std::string response = writer.write(jsonResponse);

	client.AddResponseToServer(mId, response);
}

const std::string& RequestSetTemperature::GetId()
{
	return mId;
}

const RequestType RequestSetTemperature::GetType()
{
	return RequestType::SetTemperature;
}

RequestDummy::RequestDummy(const std::string& id)
: mId(id)
{
}

RequestDummy::~RequestDummy()
{
}

void RequestDummy::Process()
{
}

const std::string& RequestDummy::GetId()
{
	return mId;
}

const RequestType RequestDummy::GetType()
{
	return RequestType::DummyRequest;
}

RequestOnline::RequestOnline(const std::string& id, const double timestamp)
: mId(id)
, mTimestamp(timestamp)
{
}

RequestOnline::~RequestOnline()
{
}

void RequestOnline::Process()
{
    TRC_DEBUG("RequestOnline::Process");
    FirebaseClient client("https://heater-control.firebaseio.com/");
    //client.SetState("Done", mTemperature + 2, "currentTime", mTemperature);
    //client.RemoveRequestFromList(mId);
    client.AddResponseToServer(mId, "{\"result\" : \"done\"}");
}

const std::string& RequestOnline::GetId()
{
    return mId;
}

const RequestType RequestOnline::GetType()
{
    return RequestType::DummyRequest;
}