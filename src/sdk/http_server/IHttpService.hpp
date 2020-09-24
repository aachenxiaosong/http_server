#ifndef  SDK_HTTP_SERVER_I_HTTP_SERVICE_H_
#define  SDK_HTTP_SERVER_I_HTTP_SERVICE_H_

#include "IHttpRequestHandler.hpp"

class IHttpService{
    private:
    string name;
    public:
    IHttpService(const char* name) ;
    virtual ~IHttpService() ;
    virtual int registerHttpHandler() = 0;
    virtual int unRegisterHttpHandler() = 0;
};

#endif  //   SDK_HTTP_SERVER_I_HTTP_SERVICE_H_