#ifndef HTTP_SERVICE_WLONG_WLONG_HTTP_SERVICE_H_
#define HTTP_SERVICE_WLONG_WLONG_HTTP_SERVICE_H_

#include "IHttpService.hpp"
#include "WlongCallLiftHttpHandler.hpp"
#include "WlongBookLiftHttpHandler.hpp"
#include "WlongTakeLiftHttpHandler.hpp"

using namespace std;

class WlongHttpService :  public IHttpService {
    private:
    WlongCallLiftHttpHandler mCallLiftHandler;
    WlongBookLiftHttpHandler mBookLiftHandler;
    WlongTakeLiftHttpHandler mTakeLiftHandler;

    public:
    WlongHttpService();
    int registerHttpHandler();
};

#endif  //  HTTP_SERVICE_WLONG_WLONG_HTTP_SERVICE_H_