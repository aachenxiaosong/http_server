#ifndef  SDK_HTTP_HTTP_CLIENT_HTTP_CLIENT_HPP_
#define  SDK_HTTP_HTTP_CLIENT_HTTP_CLIENT_HPP_

#include <string>
#include <map>

using namespace std;

class HttpClient {
public:
    static int post(const string& url, const string& content, string& result,
                    const map<string, string>& headers = map<string, string>(), int timeout = 5);
    static int get(const string& url, string& result,
                   const map<string, string>& headers = map<string, string>(), int timeout = 5);
};

#endif  //   SDK_HTTP_HTTP_CLIENT_HTTP_CLIENT_HPP_