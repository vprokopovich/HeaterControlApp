#ifndef REQUEST_H
#define REQUEST_H

#include "IRequest.h"

class RequestSetTemperature : public IRequest
{
public:

	/**
	 * 
	 */
	RequestSetTemperature(const std::string& id, const std::uint32_t temperature);
	~RequestSetTemperature();

	virtual void Process() override;
	virtual const std::string& GetId() override;
	virtual const RequestType GetType() override;

private:
	/**
	 * Default contructor is unimplemented to avoid misusing
	 */
	RequestSetTemperature();

private:
	std::string	  mId;
	std::uint32_t mTemperature;
};

class RequestDummy : public IRequest
{
public:

	/**
	 * 
	 */
	RequestDummy(const std::string& id);
	~RequestDummy();

	virtual void Process() override;
	virtual const std::string& GetId() override;
	virtual const RequestType GetType() override;

private:
	/**
	 * Default contructor is unimplemented to avoid misusing
	 */
	RequestDummy();

private:
	std::string	  mId;
};

#endif // REQUEST_H