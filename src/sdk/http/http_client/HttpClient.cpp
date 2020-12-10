/*#include "HttpClient.hpp"
#include "uni_http.h"
#include "UniLog.hpp"

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
*/

#include "HttpClient.hpp"
#include "Poco/URI.h"
#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/NetException.h"
#include "Poco/Net/AbstractHTTPRequestHandler.h"
#include "Poco/StreamCopier.h"
#include "Poco/Net/SSLManager.h"
#include "Poco/Net/AcceptCertificateHandler.h"
#include "Poco/Net/HTTPSClientSession.h"
#include "Poco/Net/Context.h"
#include "UniLog.hpp"
#include <iostream>

using namespace Poco;
using namespace Poco::Net;

#define HTTP_CLIENT_TAG "http_client"


int HttpClient::post(const string& url, const string& content, string &result,
                     const map<string, string>& headers, int timeout)
{
    int ret = -1;
    try {
        URI uri(url);
        Context::Ptr m_context = new Context(Context::CLIENT_USE, "", "", "", Context::VERIFY_NONE, 9, false);
        HTTPClientSession session(uri.getHost(), uri.getPort());
        HTTPRequest request(HTTPRequest::HTTP_POST, uri.getPathAndQuery());
        string s_headers("");
        for (auto i : headers) {
            request.set(i.first, i.second);
            s_headers.append("(" + i.first + "," + i.second + ")");
        }
        if (timeout > 0)
        {
            Timespan time_span(timeout, 0);
            session.setTimeout(time_span);
        }

        //request.setContentLength(content.length());
        session.sendRequest(request) << content;
        LOGT(HTTP_CLIENT_TAG, "post request sent -----\nurl:%s\nheaders:%s\ncontent:%s\ntimeout:%d\n",
             url.c_str(), s_headers.c_str(), content.c_str(), timeout);
        HTTPResponse response;
        istream &is = session.receiveResponse(response);
        HTTPResponse::HTTPStatus status = response.getStatus();
        if (HTTPResponse::HTTPStatus::HTTP_OK == status) {
            StreamCopier::copyToString(is, result);
            ret = 0;
            LOGT(HTTP_CLIENT_TAG, "post response recved ----\nstatus:%d\nresult:%s\n",
                 status, result.c_str());
        } else {
            LOGE(HTTP_CLIENT_TAG, "post request failed, status:" + to_string(status));
        }
        
    } catch (Poco::Exception &e) {
        LOGE(HTTP_CLIENT_TAG, "post request failed, exception:" + e.message());
    }
    return ret;
}

int HttpClient::get(const string& url, string& result,
                    const map<string, string>& headers, int timeout)
{
    int ret = -1;
    try {
        URI uri(url);
        HTTPClientSession session(uri.getHost(), uri.getPort());
        HTTPRequest request(HTTPRequest::HTTP_GET, uri.getPathAndQuery());
        string s_headers("");
        for (auto i : headers) {
            request.set(i.first, i.second);
            s_headers.append("(" + i.first + "," + i.second + ")");
        }
        if (timeout > 0)
        {
            Timespan time_span(timeout, 0);
            session.setTimeout(time_span);
        }
        session.sendRequest(request);
        LOGT(HTTP_CLIENT_TAG, "get request sent -----\nurl:%s\nheaders:%s\ntimeout:%d\n",
             url.c_str(), s_headers.c_str(), timeout);
        HTTPResponse response;
        istream &is = session.receiveResponse(response);
        HTTPResponse::HTTPStatus status = response.getStatus();
        if (HTTPResponse::HTTPStatus::HTTP_OK == status) {
            StreamCopier::copyToString(is, result);
            ret = 0;
            LOGT(HTTP_CLIENT_TAG, "get response recved ----\nstatus:%d\nresult:%s\n",
                 status, result.c_str());
        } else {
            LOGE(HTTP_CLIENT_TAG, "get request failed, status:" + to_string(status));
        }
    } catch (Poco::Exception &e) {
        LOGE(HTTP_CLIENT_TAG, "get request failed, exception:" + e.message());
    }
    return ret;
}

int HttpClient::postHttps(const string& url, const string& content, string &result,
                          const map<string, string>& headers, int timeout)  
{
    int ret = -1;
    try {
        SSLManager::InvalidCertificateHandlerPtr handlerPtr(new AcceptCertificateHandler(false));
        Context::Ptr context = new Context(Context::CLIENT_USE, "");
        URI uri(url);
        HTTPSClientSession session(uri.getHost(), uri.getPort(), context);
        HTTPRequest request(HTTPRequest::HTTP_POST, uri.getPathAndQuery());
        request.setContentLength(content.length());
        string s_headers("");
        for (auto i : headers) {
            request.set(i.first, i.second);
            s_headers.append("(" + i.first + "," + i.second + ")");
        }
        if (timeout > 0)
        {
            Timespan time_span(timeout, 0);
            session.setTimeout(time_span);
        }
        //request.setContentLength(content.length());
        session.sendRequest(request) << content;
        LOGT(HTTP_CLIENT_TAG, "post request sent -----\nurl:%s\nheaders:%s\ncontent:%s\ntimeout:%d\n",
             url.c_str(), s_headers.c_str(), content.c_str(), timeout);
        HTTPResponse response;
        istream &is = session.receiveResponse(response);
        HTTPResponse::HTTPStatus status = response.getStatus();
        if (HTTPResponse::HTTPStatus::HTTP_OK == status) {
            StreamCopier::copyToString(is, result);
            ret = 0;
            LOGT(HTTP_CLIENT_TAG, "post response recved ----\nstatus:%d\nresult:%s\n",
                 status, result.c_str());
        } else {
            LOGE(HTTP_CLIENT_TAG, "post request failed, status:" + to_string(status));
        }
        
    } catch (Poco::Exception &e) {
        LOGE(HTTP_CLIENT_TAG, "post request failed, exception:" + e.message());
    }
    return ret;
}

int HttpClient::getHttps(const string& url, string& result,
                         const map<string, string>& headers, int timeout)
{
    int ret = -1;
    try {
        SSLManager::InvalidCertificateHandlerPtr handlerPtr(new AcceptCertificateHandler(false));
        Context::Ptr context = new Context(Context::CLIENT_USE, "");
        SSLManager::instance().initializeClient(nullptr, handlerPtr, context);
        URI uri(url);
        HTTPSClientSession session(context);
        session.setHost(uri.getHost());
        session.setPort(uri.getPort());
        HTTPRequest request(HTTPRequest::HTTP_GET, uri.getPathAndQuery());
        string s_headers("");
        for (auto i : headers) {
            request.set(i.first, i.second);
            s_headers.append("(" + i.first + "," + i.second + ")");
        }
        if (timeout > 0)
        {
            Timespan time_span(timeout, 0);
            session.setTimeout(time_span);
        }
        session.sendRequest(request);
        LOGT(HTTP_CLIENT_TAG, "get request sent -----\nurl:%s\nheaders:%s\ntimeout:%d\n",
             url.c_str(), s_headers.c_str(), timeout);
        HTTPResponse response;
        istream &is = session.receiveResponse(response);
        HTTPResponse::HTTPStatus status = response.getStatus();
        if (HTTPResponse::HTTPStatus::HTTP_OK == status) {
            StreamCopier::copyToString(is, result);
            ret = 0;
            LOGT(HTTP_CLIENT_TAG, "get response recved ----\nstatus:%d\nresult:%s\n",
                 status, result.c_str());
        } else {
            LOGE(HTTP_CLIENT_TAG, "get request failed, status:" + to_string(status));
        }
    } catch (Poco::Exception &e) {
        LOGE(HTTP_CLIENT_TAG, "get request failed, exception:" + e.message());
    }
    return ret;
}