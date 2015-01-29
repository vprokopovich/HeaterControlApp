#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <exception>

class JsonParsingException : public std::exception
{
    virtual const char* what() const throw()
    {
        return "Json document parsing error";
    }
};

class CurlException : public std::exception
{
public:
    CurlException(const char* errMsg)
    : mpErrMsg(errMsg)
    {}

    virtual const char* what() const throw()
    {
        return mpErrMsg;
    }

    const char* mpErrMsg;
};

#endif // EXCEPTIONS_H