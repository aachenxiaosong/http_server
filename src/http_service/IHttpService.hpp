#ifndef HTTP_SERVICE_I_HTTP_SERVICE_H_
#define HTTP_SERVICE_I_HTTP_SERVICE_H_

#include "IHttpRequestHandler.hpp"

class IHttpService{
    private:
    string name;
    public:
    IHttpService(const char* name) ;
    virtual ~IHttpService() ;
    virtual int registerHttpHandler() = 0;
};

#endif  //  HTTP_SERVICE_I_HTTP_SERVICE_H_