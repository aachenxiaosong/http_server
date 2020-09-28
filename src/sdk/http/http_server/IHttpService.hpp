#ifndef  SDK_HTTP_HTTP_SERVER_I_HTTP_SERVICE_HPP_
#define  SDK_HTTP_HTTP_SERVER_I_HTTP_SERVICE_HPP_

#include "IHttpRequestHandler.hpp"

class IHttpService{
    private:
    string name;
    public:
    IHttpService(const char* name);
    virtual ~IHttpService();
    virtual int registerHttpHandler() = 0;
    virtual int unRegisterHttpHandler() = 0;
};

#endif  //   SDK_HTTP_HTTP_SERVER_I_HTTP_SERVICE_HPP_