/*
 * All header file for c language MUST be include : in extern "C" { }
 */
extern "C" {
#include "../src/uni_interruptable.c"
}

static InterruptHandle g_handle;
static pthread_t g_pid;
static int g_ms;

H2UNIT(interruptable) {
  void setup() {
    g_handle = InterruptCreate();;
  }
  void teardown() {
    InterruptDestroy(g_handle);
  }
};

static void *_break_sleep_tsk(void *args) {
  uni_msleep(g_ms / 2);
  InterruptableBreak(g_handle);
}

static int _test_break(int ms) {
  int rc;
  g_ms = ms;
  pthread_create(&g_pid, NULL, _break_sleep_tsk, NULL);
  rc = InterruptableSleep(g_handle, ms);
  uni_msleep(5);
  return rc;
}

H2CASE(interruptable, "interruptable sleep test") {
  int rc;
  rc = _test_break(0); 
  H2EQ_MATH(0, rc);
  rc = _test_break(10); 
  H2EQ_MATH(1, rc);
  rc = _test_break(100); 
  H2EQ_MATH(1, rc);
  rc = _test_break(-1); 
  H2EQ_MATH(-1, rc);
}
