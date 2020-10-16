#ifndef HTTP_SERVICE_SERVICES_RILI_HTTP_SERVICE_H_
#define HTTP_SERVICE_SERVICES_RILI_HTTP_SERVICE_H_

#include "IHttpService.hpp"
#include "RiliCallLiftHttpHandler.hpp"
#include "RiliBookLiftHttpHandler.hpp"
#include "RiliTakeLiftHttpHandler.hpp"

using namespace std;

class RiliHttpService :  public IHttpService {
    private:
    RiliCallLiftHttpHandler mCallLiftHandler;
    RiliBookLiftHttpHandler mBookLiftHandler;
    RiliTakeLiftHttpHandler mTakeLiftHandler;

    public:
    RiliHttpService();
    int registerHttpHandler();
    int unRegisterHttpHandler();
};

#endif  //  HTTP_SERVICE_SERVICES_RILI_HTTP_SERVICE_H_