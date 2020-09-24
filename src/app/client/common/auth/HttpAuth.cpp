#include "HttpAuth.hpp"
#include "uni_auth_http.h"
#include "uni_log.h"

const char* HttpAuth::mLogTag = "http_auth";
int HttpAuth::mValidTime = 0;
string HttpAuth::mToken = "";
thread* HttpAuth::mThread = NULL;

void HttpAuth::tokenUpdateTask() {
    while (1) {
        if (HttpGetTokenCacheUpdate() == 0) {
            Token *token = HttpGetToken();
            if (NULL != token) {
                mValidTime = token->valid_time;
                mToken = token->token;
                HttpGetTokenFree(token);
                sleep(mValidTime / 2);
                continue;
            }
        }
        sleep(10);
    }
}

int HttpAuth::init() {
    mThread = new thread(tokenUpdateTask);
    if (mThread == NULL) {
        LOGE(mLogTag, "HttpAuth create token update thread failed");
        return -1;
    }
    return 0;
}
string HttpAuth::getToken() {
    return mToken;
}