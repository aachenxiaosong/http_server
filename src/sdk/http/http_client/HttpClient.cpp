#include "HttpClient.hpp"
#include "uni_http.h"
#include "uni_log.h"

#define HTTP_CLIENT_TAG "http_client"

int HttpClient::post(const string& url, const string& content, string &result,
                     const map<string, string>& headers, int timeout) {
    int ret;
    const char *c_url = url.c_str();
    const char *c_content = content.c_str();
    const char *c_headers[10][2] = {};
    char *c_result = NULL;
    int c_header_num = 0;
    string s_headers("");
    map<string, string>::const_iterator iter;
    iter = headers.begin();
    while (iter != headers.end() && c_header_num < 10) {
        c_headers[c_header_num][0] = iter->first.c_str();
        c_headers[c_header_num][1] = iter->second.c_str();
        s_headers.append("(");
        s_headers.append(c_headers[c_header_num][0]);
        s_headers.append(",");
        s_headers.append(c_headers[c_header_num][1]);
        s_headers.append(") ");
        LOGT(HTTP_CLIENT_TAG, "%s:%s", c_headers[c_header_num][0], c_headers[c_header_num][1]);
        c_header_num++;
        iter++;
    }
    LOGT(HTTP_CLIENT_TAG, "post request sent -----\nurl:%s\nheaders:%s\ncontent:%s\ntimeout:%d\n",
         c_url, s_headers.c_str(), c_content, timeout);
    ret = HttpPostWithHeadersTimeout(c_url, c_content, c_headers, c_header_num, timeout, &c_result);
    LOGT(HTTP_CLIENT_TAG, "post response recved ----\nret:%d\nresult:%s\n",
         ret, c_result == NULL ? "NULL" : c_result);
    if (c_result != NULL) {
        result = c_result;
        uni_free(c_result);
    }
    return ret;
}

int HttpClient::get(const string& url, string& result,
                    const map<string, string>& headers, int timeout) {
    int ret;
    const char *c_url = url.c_str();
    const char *c_headers[10][2] = {};
    char *c_result = NULL;
    int c_header_num = 0;
    string s_headers("");
    map<string, string>::const_iterator iter;
    iter = headers.begin();
    while (iter != headers.end()) {
        c_headers[c_header_num][0] = iter->first.c_str();
        c_headers[c_header_num][1] = iter->second.c_str();
        s_headers.append("(");
        s_headers.append(c_headers[c_header_num][0]);
        s_headers.append(",");
        s_headers.append(c_headers[c_header_num][1]);
        s_headers.append(") ");
        c_header_num++;
    }
    LOGT(HTTP_CLIENT_TAG, "get request sent -----\nurl:%s\nheaders:%s\ntimeout:%d\n",
         c_url, s_headers.c_str(), timeout);
    ret = HttpGetWithHeadersTimeout(c_url, c_headers, c_header_num, timeout, &c_result);
    LOGT(HTTP_CLIENT_TAG, "get response recved ----\nret:%d\nresult:%s\n",
         ret, c_result == NULL ? "NULL" : c_result);
    if (c_result != NULL) {
        result = c_result;
        uni_free(c_result);
    }
    return ret;
}