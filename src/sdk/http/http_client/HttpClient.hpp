#ifndef  SDK_HTTP_HTTP_CLIENT_HTTP_CLIENT_HPP_
#define  SDK_HTTP_HTTP_CLIENT_HTTP_CLIENT_HPP_

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

#endif  //   SDK_HTTP_HTTP_CLIENT_HTTP_CLIENT_HPP_