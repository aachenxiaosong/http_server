/*
 * All header file for c language MUST be include : in extern "C" { }
 */
extern "C" {
#include "../src/uni_arpt.c"
}

H2UNIT(arpt) {
  void setup(void) {
  }
  void teardown(void) {
  }
};

H2CASE(arpt, "init test") {
  int ret;
  /* 1. without debug_open and debug_more */
  ret = ArptInit("./");
  H2EQ_MATH(0, ret);
  H2EQ_MATH(FALSE, g_arpt.is_open);
  H2EQ_MATH(FALSE, g_arpt.is_more);
  H2EQ_TRUE(g_arpt.fd <= 0);
  ArptFinal();
  /* 2. with debug_open */
  system("touch debug_open");
  ret = ArptInit("./");
  H2EQ_MATH(0, ret);
  H2EQ_MATH(TRUE, g_arpt.is_open);
  H2EQ_MATH(FALSE, g_arpt.is_more);
  H2EQ_TRUE(g_arpt.fd > 0);
  ArptFinal();
  /* 3. with debug_open and debug_more */
  system("touch debug_more");
  ret = ArptInit("./");
  H2EQ_MATH(0, ret);
  H2EQ_MATH(TRUE, g_arpt.is_open);
  H2EQ_MATH(TRUE, g_arpt.is_more);
  H2EQ_TRUE(g_arpt.fd > 0);
  ArptFinal();
  system("rm -f debug_open debug_more asr_log");
  /* 4. invalid input */
  ret = ArptInit("12345678901234567890123456789012");
  H2EQ_MATH(-1, ret);
  ret = ArptInit(NULL);
  H2EQ_MATH(-1, ret);
}

H2CASE(arpt, "write test") {
  int ret;
  /* 1. without debug_open and debug_more */
  ArptInit("./");
  ret = ArptWrite("test_content", FALSE);
  H2EQ_MATH(-1, ret);
  ret = ArptWrite("test_content", TRUE);
  H2EQ_MATH(-1, ret);
  ArptFinal();
  /* 2. with debug_open */
  system("touch debug_open");
  ArptInit("./");
  ret = ArptWrite("test_content", FALSE);
  H2EQ_MATH(0, ret);
  ret = ArptWrite("test_content", TRUE);
  H2EQ_MATH(-1, ret);
  ArptFinal();
  /* 3. with debug_open and debug_more */
  system("touch debug_more");
  ArptInit("./");
  ret = ArptWrite("test_content", FALSE);
  H2EQ_MATH(0, ret);
  ret = ArptWrite("test_content", TRUE);
  H2EQ_MATH(0, ret);
  ArptFinal();
  system("rm -f debug_open debug_more asr_log");
}
