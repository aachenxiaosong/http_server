/*
 * All header file for c language MUST be include : in extern "C" { }
 */
extern "C" {
#include "../src/uni_sha.c"
}

H2UNIT(sha1) {
  void setup() {}
  void teardown() {}
};

H2CASE(sha1, "get sha1 result string") {
  int ret;
  char data[1024] = {'\0'};
  char result[64] = {'\0'};
  uni_snprintf(data, sizeof(data), "hello world");
  ret = SHA1DigestStr(data, result);
  H2EQ_MATH(SHA_DIGEST_LENGTH * 2 + 1, ret);
  H2EQ_STRCMP("2AAE6C35C94FCFB415DBE95F408B9CE91EE846ED", result);
  uni_snprintf(data, sizeof(data), "1");
  ret = SHA1DigestStr(data, result);
  H2EQ_MATH(SHA_DIGEST_LENGTH * 2 + 1, ret);
  H2EQ_STRCMP("356A192B7913B04C54574D18C28D46E6395428AB", result);
  uni_snprintf(data, sizeof(data), "你好魔方");
  ret = SHA1DigestStr(data, result);
  H2EQ_MATH(SHA_DIGEST_LENGTH * 2 + 1, ret);
  H2EQ_STRCMP("A376F6953CF0805BD2A68804DC635D6B3D854D10", result);
  uni_snprintf(data, sizeof(data), "1234你好魔方abcd");
  ret = SHA1DigestStr(data, result);
  H2EQ_MATH(SHA_DIGEST_LENGTH * 2 + 1, ret);
  H2EQ_STRCMP("9C8EBE0D2A36101F33E7EC0C886E40AE334082B4", result);
  uni_snprintf(data, sizeof(data), "1234     你好魔方abcd\t\t\n\n");
  ret = SHA1DigestStr(data, result);
  H2EQ_MATH(SHA_DIGEST_LENGTH * 2 + 1, ret);
  H2EQ_STRCMP("840223C0938CDFD3242F1231C7D074A78CD65A02", result);
}
