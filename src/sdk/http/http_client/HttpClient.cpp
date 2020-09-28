#include "HttpClient.hpp"
#include "uni_http.h"
#include "uni_log.h"

const char *HttpClient::mLogTag = "http_client";

int HttpClient::post(string url, string content, string &result,
                     map<string, string> headers, int timeout) {
    int ret;
    const char *c_url = url.c_str();
    const char *c_content = content.c_str();
    const char *c_headers[10][2] = {};
    char *c_result = NULL;
    int c_header_num = 0;
    string s_headers("");
    map<string, string>::iterator iter;
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
    LOGT(mLogTag, "post request sent -----\n"
                  "url:%s\n"
                  "headers:%s\n"
                  "content:%s\n"
                  "timeout:%d\n",
                  c_url, s_headers.c_str(), c_content, timeout);
    ret = HttpPostWithHeadersTimeout(c_url, c_content, c_headers, c_header_num, timeout, &c_result);
    LOGT(mLogTag, "post response recved ----\n"
                  "ret:%d\n"
                  "result:%s\n",
                  ret, c_result == NULL ? "NULL" : c_result);
    if (c_result != NULL) {
        result = c_result;
        uni_free(c_result);
    }
    return ret;
}

int HttpClient::get(string url, string &result,
                    map<string, string> headers, int timeout) {
    int ret;
    const char *c_url = url.c_str();
    const char *c_headers[10][2] = {};
    char *c_result = NULL;
    int c_header_num = 0;
    string s_headers("");
    map<string, string>::iterator iter;
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
    LOGT(mLogTag, "get request sent -----\n"
                  "url:%s\n"
                  "headers:%s\n"
                  "timeout:%d\n",
                  c_url, s_headers.c_str(), timeout);
    ret = HttpGetWithHeadersTimeout(c_url, c_headers, c_header_num, timeout, &c_result);
    LOGT(mLogTag, "get response recved ----\n"
                  "ret:%d\n"
                  "result:%s\n",
                  ret, c_result == NULL ? "NULL" : c_result);
    if (c_result != NULL) {
        result = c_result;
        uni_free(c_result);
    }
    return ret;
}