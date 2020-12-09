#pragma once

#include <string>
#include <map>

using namespace std;

class HttpClient {
public:
    static int post(const string& url, const string& content, string& result,
                    const map<string, string>& headers = map<string, string>(), int timeout = 5);
    static int get(const string& url, string& result,
                   const map<string, string>& headers = map<string, string>(), int timeout = 5);
    static int postHttps(const string& url, const string& content, string& result,
                         const map<string, string>& headers = map<string, string>(), int timeout = 5);
    static int getHttps(const string& url, string& result,
                        const map<string, string>& headers = map<string, string>(), int timeout = 5);
                   
};