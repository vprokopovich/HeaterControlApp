#ifndef IREQUEST_H
#define IREQUEST_H

#include <memory>
#include <vector>
#include <map>

enum class RequestType
{
	None,
	SetTemperature,
	DummyRequest
};

class IRequest
{
public:
	/**
	 * Processes request
	 */
	virtual void Process() = 0;

	/**
	 * Provides request Id
	 */
	virtual const std::string& GetId() = 0;

	virtual const RequestType GetType() = 0;
};

typedef std::shared_ptr<IRequest> IRequestPtrS;
typedef std::unique_ptr<IRequest> IRequestPtrU;
typedef std::vector<IRequestPtrS> RequestsVector;

#endif // IREQUEST_H