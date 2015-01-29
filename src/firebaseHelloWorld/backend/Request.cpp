#include "Request.h"
#include <CTrace.h>
#include "FirebaseClient.h"

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
	client.SetState("Done", mTemperature + 2, "currentTime", mTemperature);
	client.RemoveRequestFromList(mId);
	client.AddResponseToServer(mId, "{\"result\" : \"done\"}");
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