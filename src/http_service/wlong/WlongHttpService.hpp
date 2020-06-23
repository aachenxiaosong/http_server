#ifndef HTTP_SERVICE_WLONG_WLONG_HTTP_SERVICE_H_
#define HTTP_SERVICE_WLONG_WLONG_HTTP_SERVICE_H_

#include "IHttpService.hpp"
#include "WlongCallLiftHttpHandler.hpp"
#include "WlongCallLiftOutHttpHandler.hpp"
#include "WlongBookLiftHttpHandler.hpp"
#include "WlongBookLift1HttpHandler.hpp"
#include "WlongTakeLiftHttpHandler.hpp"

using namespace std;

class WlongHttpService :  public IHttpService {
    private:
    WlongCallLiftHttpHandler mCallLiftHandler;
    WlongCallLiftOutHttpHandler mCallLiftOutHandler;
    WlongBookLiftHttpHandler mBookLiftHandler;
    WlongBookLift1HttpHandler mBookLift1Handler;
    WlongTakeLiftHttpHandler mTakeLiftHandler;

    public:
    WlongHttpService();
    int registerHttpHandler();
};

#endif  //  HTTP_SERVICE_WLONG_WLONG_HTTP_SERVICE_H_