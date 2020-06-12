#ifndef SERVICE_RILI_RILI_HTTP_SERVICE_H_
#define SERVICE_RILI_RILI_HTTP_SERVICE_H_

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
};

#endif  //  SERVICE_RILI_RILI_HTTP_SERVICE_H_