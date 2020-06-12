#ifndef SERVICE_I_HTTP_SERVICE_H_
#define SERVICE_I_HTTP_SERVICE_H_

#include "IHttpRequestHandler.hpp"

class IHttpService{
    private:
    string name;
    public:
    IHttpService(const char* name) ;
    virtual int registerHttpHandler() = 0;
};

#endif