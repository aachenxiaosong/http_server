#ifndef  SDK_HTTP_CLIENT_HTTP_CLIENT_H_
#define  SDK_HTTP_CLIENT_HTTP_CLIENT_H_

#include <string>
#include <map>

using namespace std;

class HttpClient {
private:
    static const char* mLogTag;
public:
    static int post(string url, string content, string &result,
                    map<string, string> headers = map<string, string>(), int timeout = 5);
    static int get(string url, string &result,
                   map<string, string> headers = map<string, string>(), int timeout = 5);
};

#endif  //   SDK_HTTP_CLIENT_HTTP_CLIENT_H_