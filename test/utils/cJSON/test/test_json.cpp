/*
 * All header file for c language MUST be include : in extern "C" { }
 */
extern "C" {
#include "../src/uni_json.c"
}

/*
 * H2UNIT is a unit test Suite, which contains several test cases.
 */
H2UNIT(json) {
  /*
   * setup() will be executed before every test case belong to this suite.
   * Typically it is used to prepare preconditions.
   */
  void setup() {
  }

  /*
   * teardown() will be executed after every test case belong to this suite.
   * whatever test case passed or failed.
   * Typically it is used to release resource.
   */
  void teardown() {
  }
};

static const char *json1 = "{\"key_s\":\"str\", \"key_i\":100, \"key_d\":100.5}";
static const char *json2 = "{\"key_s\": {\"key_s1\": {\"key_s2\": \"str1\"}}, \
                             \"key_i\": {\"key_i1\": {\"key_i2\": 101}}, \
                             \"key_d\": {\"key_d1\": {\"key_d2\": 101.5}}}";
static const char *json3 = "{\"key_s\": {\"<key_s1>\": {\"key_s2\": \"str1\"}}, \
                             \"key_i\": {\"*key_i1*\": {\"key_i2\": 101}}, \
                             \"key_d\": {\"(key_d1)\": {\"key_d2\": 101.5}}}";
static const char *json4 = "{\"key_s\": {\"key_s1\": {\"dist\":100, \"key_s2\": \"str1\"}}, \
                             \"key_i\": {\"key_i1\": {\"dist\":101.5, \"key_i2\": 101}}, \
                             \"key_d\": {\"key_d1\": {\"dist\":\"str\", \"key_d2\": 101.5}}}";
static const char *json5 = "{\"key_s\": {\"key_s1\": {\"key_s2\": [\"str1\", \"str2\", \"str2\"]}}, \
                             \"key_i\": {\"key_i1\": [{\"key_i2\": 101}, {\"key_i2\": 101}]}, \
                             \"key_d\": [{\"key_d1\": {\"key_d2\": 101.5}}, {\"key_d1\": {\"key_d2\": 101.5}}]}";
static const char *json6 = "{\"key_s\":100, \"key_i\":100.5, \"key_d\":\"not double\"}";
H2CASE(json, "JsonReadItem test") {
  cJSON *root;
  char *str = NULL;
  int i = 0;
  double d = 0.0;
  int ret;
  /* 1. basic test with depth 1 */
  root = cJSON_Parse(json1);
  ret = JsonReadItemInt(root, "key_i", &i);
  H2EQ_TRUE(ret == 0);
  H2EQ_TRUE(i == 100);
  ret = JsonCheckItemInt(root, "key_i");
  H2EQ_TRUE(ret == 0);
  ret = JsonReadItemDouble(root, "key_d", &d);
  H2EQ_TRUE(ret == 0);
  H2EQ_TRUE(d == 100.5);
  ret = JsonCheckItemDouble(root, "key_d");
  H2EQ_TRUE(ret == 0);
  ret = JsonReadItemString(root, "key_s", &str);
  H2EQ_TRUE(ret == 0);
  H2EQ_STRCMP("str", str);
  ret = JsonCheckItemString(root, "key_s");
  H2EQ_TRUE(ret == 0);
  cJSON_Delete(root);
  uni_free(str);
  str = NULL;

  /* 2. basic test with depth 3 */
  root = cJSON_Parse(json2);
  ret = JsonReadItemInt(root, "key_i.key_i1.key_i2", &i);
  H2EQ_TRUE(ret == 0);
  H2EQ_TRUE(i == 101);
  ret = JsonCheckItemInt(root, "key_i.key_i1.key_i2");
  H2EQ_TRUE(ret == 0);
  ret = JsonReadItemDouble(root, "key_d.key_d1.key_d2", &d);
  H2EQ_TRUE(ret == 0);
  H2EQ_TRUE(d == 101.5);
  ret = JsonCheckItemDouble(root, "key_d.key_d1.key_d2");
  H2EQ_TRUE(ret == 0);
  ret = JsonReadItemString(root, "key_s.key_s1.key_s2", &str);
  H2EQ_TRUE(ret == 0);
  H2EQ_STRCMP("str1", str);
  ret = JsonCheckItemString(root, "key_s.key_s1.key_s2");
  H2EQ_TRUE(ret == 0);
  cJSON_Delete(root);
  uni_free(str);
  str = NULL;

  /* 3. basic test with special charactors */
  root = cJSON_Parse(json3);
  ret = JsonReadItemInt(root, "key_i.*key_i1*.key_i2", &i);
  H2EQ_TRUE(ret == 0);
  H2EQ_TRUE(i == 101);
  ret = JsonCheckItemInt(root, "key_i.*key_i1*.key_i2");
  H2EQ_TRUE(ret == 0);
  ret = JsonReadItemDouble(root, "key_d.(key_d1).key_d2", &d);
  H2EQ_TRUE(ret == 0);
  H2EQ_TRUE(d == 101.5);
  ret = JsonCheckItemDouble(root, "key_d.(key_d1).key_d2");
  H2EQ_TRUE(ret == 0);
  ret = JsonReadItemString(root, "key_s.<key_s1>.key_s2", &str);
  H2EQ_TRUE(ret == 0);
  H2EQ_STRCMP("str1", str);
  ret = JsonCheckItemString(root, "key_s.<key_s1>.key_s2");
  H2EQ_TRUE(ret == 0);
  cJSON_Delete(root);
  uni_free(str);
  str = NULL;

  /* 4. basic test with disrubtance term */
  root = cJSON_Parse(json4);
  ret = JsonReadItemInt(root, "key_i.key_i1.key_i2", &i);
  H2EQ_TRUE(ret == 0);
  H2EQ_TRUE(i == 101);
  ret = JsonCheckItemInt(root, "key_i.key_i1.key_i2");
  H2EQ_TRUE(ret == 0);
  ret = JsonReadItemDouble(root, "key_d.key_d1.key_d2", &d);
  H2EQ_TRUE(ret == 0);
  H2EQ_TRUE(d == 101.5);
  ret = JsonCheckItemDouble(root, "key_d.key_d1.key_d2");
  H2EQ_TRUE(ret == 0);
  ret = JsonReadItemString(root, "key_s.key_s1.key_s2", &str);
  H2EQ_TRUE(ret == 0);
  H2EQ_STRCMP("str1", str);
  ret = JsonCheckItemString(root, "key_s.key_s1.key_s2");
  H2EQ_TRUE(ret == 0);
  cJSON_Delete(root);
  uni_free(str);
  str = NULL;

  /* 5. basic test with array */
  root = cJSON_Parse(json5);
  ret = JsonReadItemInt(root, "key_i.key_i1[0].key_i2", &i);
  H2EQ_TRUE(ret == 0);
  H2EQ_TRUE(i == 101);
  ret = JsonCheckItemInt(root, "key_i.key_i1[0].key_i2");
  H2EQ_TRUE(ret == 0);
  ret = JsonReadItemDouble(root, "key_d[1].key_d1.key_d2", &d);
  H2EQ_TRUE(ret == 0);
  H2EQ_TRUE(d == 101.5);
  ret = JsonCheckItemDouble(root, "key_d[1].key_d1.key_d2");
  H2EQ_TRUE(ret == 0);
  ret = JsonReadItemString(root, "key_s.key_s1.key_s2[2]", &str);
  H2EQ_TRUE(ret == 0);
  H2EQ_STRCMP("str2", str);
  ret = JsonCheckItemString(root, "key_s.key_s1.key_s2[2]");
  H2EQ_TRUE(ret == 0);
  cJSON_Delete(root);
  uni_free(str);
  str = NULL;

  /* 6. corner cases */
  ret = JsonReadItemInt(NULL, "key_i", &i);
  H2EQ_TRUE(ret < 0);
  ret = JsonCheckItemInt(NULL, "key_i");
  H2EQ_TRUE(ret < 0);
  root = cJSON_Parse(json1);
  ret = JsonReadItemInt(root, "key_ii", &i);
  H2EQ_TRUE(ret < 0);
  ret = JsonCheckItemInt(root, "key_ii");
  H2EQ_TRUE(ret < 0);
  cJSON_Delete(root);
  root = cJSON_Parse(json6);
  ret = JsonReadItemInt(root, "key_i", &i);
  H2EQ_TRUE(ret < 0);
  ret = JsonCheckItemInt(root, "key_i");
  H2EQ_TRUE(ret < 0);
  ret = JsonReadItemDouble(root, "key_d", &d);
  H2EQ_TRUE(ret < 0);
  ret = JsonCheckItemDouble(root, "key_d");
  H2EQ_TRUE(ret < 0);
  ret = JsonReadItemString(root, "key_s", &str);
  H2EQ_TRUE(ret < 0);
  ret = JsonCheckItemString(root, "key_s");
  H2EQ_TRUE(ret < 0);
  cJSON_Delete(root);
  uni_free(str);
  str = NULL;
}


