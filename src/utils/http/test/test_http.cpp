/*
 * All header file for c language MUST be include : in extern "C" { }
 */
extern "C" {
#include "../src/uni_http.c"
}

H2UNIT(http) {
  void setup() {
  }

  void teardown() {
  }
};

#define URL g_server_url

static const char *g_headers[2][2] = {{"key1", "value1"}, {"key2", "value2"}};
static const char *g_server_url = "http://127.0.0.1:8000";
static const char *g_baidu_url = "http://www.baidu.com";

H2CASE(http, "http_get") {
  int ret;
  char *result = NULL;
  /* test without headers and timeout */
  ret = HttpGet(URL, &result);
  H2EQ_MATH(0, ret);
  H2EQ_TRUE(result != NULL);
  uni_free(result);
  result = NULL;
  /* test with headers */
  ret = HttpGetWithHeaders(URL, g_headers, 2, &result);
  H2EQ_MATH(0, ret);
  H2EQ_TRUE(result != NULL);
  uni_free(result);
  result = NULL;
  /* test with timeout */
  ret = HttpGetWithTimeout(URL, 5, &result);
  H2EQ_MATH(0, ret);
  H2EQ_TRUE(result != NULL);
  uni_free(result);
  result = NULL;
}

H2CASE(http, "http_post") {
  int ret;
  const char *content = "&index1:content1&index2:content2";
  /* test without headers and timeout */
  ret = HttpPost(URL, content);
  H2EQ_MATH(0, ret);
  /* test with headers */
  ret = HttpPostWithHeaders(URL, content, g_headers, 2);
  H2EQ_MATH(0, ret);
  /* test with timeout */
  ret = HttpPostWithTimeout(URL, content, 5);
  H2EQ_MATH(0, ret);
}
