/*
 * All header file for c language MUST be include : in extern "C" { }
 */
extern "C" {
#include "../src/uni_log.c"
}

static unsigned long _get_now_str_stub(char *buf, int len) {
  struct tm local;
  struct timeval tv;
  local.tm_hour = 0;
  local.tm_min = 0;
  local.tm_sec = 0;
  tv.tv_sec = 0;
  tv.tv_usec = 0;
  uni_snprintf(buf, len, "%02d:%02d:%02d.%06"PRId64" ",
      local.tm_hour,
      local.tm_min,
      local.tm_sec,
      (int64_t)tv.tv_usec);
  return tv.tv_sec * 1000 + (tv.tv_usec / 1000);
}

static void _get_thread_id_str_stub(char *buf, int len) {
  uni_snprintf(buf, len, "%x", 0);
}

static char g_buf[LOG_BUFFER_LEN];

static int _sync_write_process_stub(LogLevel level, const char *tags,
                                          const char *function, int line,
                                          char *fmt, va_list args) {
  char buf[LOG_BUFFER_LEN];
  _log_assemble(buf, level, tags, function, line, fmt, args);
  uni_snprintf(g_buf, sizeof(g_buf), "%s", buf);
  return 0;
}

H2UNIT(log) {
  void setup() {
    H2STUB(_get_now_str, _get_now_str_stub);
    H2STUB(_get_thread_id_str, _get_thread_id_str_stub);
    H2STUB(_sync_write_process, _sync_write_process_stub);
  }

  void teardown() {
  }
};

H2CASE(log, "log_init_test") {
  LogConfig log_config = {1, 1, 1, 1, 0, N_LOG_ALL};
  LogInitialize(&log_config);
  H2EQ_TRUE(g_log_config.enable_time);
  H2EQ_TRUE(g_log_config.enable_thread_id);
  H2EQ_TRUE(g_log_config.enable_function_line);
  H2EQ_TRUE(g_log_config.enable_sync);
  H2EQ_MATH(N_LOG_ALL, g_log_config.set_level);
  LogFinalize();
}

H2CASE(log, "log_write_test") {
  const char *function = __FUNCTION__;
  const int line = __LINE__;
  char buf[LOG_BUFFER_LEN];
  /* normal Track log */
  LogConfig log_config1 = {1, 1, 1, 1, 0, N_LOG_ALL};
  LogInitialize(&log_config1);
  uni_snprintf(buf, sizeof(buf), "\033[0m\033[42;33m[T]\033[0m 00:00:00.000000 0<test>%s:%d->"
         "test 10\n", function, line);
  LogWrite(N_LOG_TRACK, "test", function, line, "test %d", 10);
  H2EQ_STRCMP(buf, g_buf);
  LogFinalize();

  /* log without time */
  LogConfig log_config2 = {0, 1, 1, 1, 0, N_LOG_ALL};
  LogInitialize(&log_config2);
  uni_snprintf(buf, sizeof(buf), "\033[0m\033[42;33m[T]\033[0m 0<test>%s:%d->"
         "test 10\n", function, line);
  LogWrite(N_LOG_TRACK, "test", function, line, "test %d", 10);
  H2EQ_STRCMP(buf, g_buf);
  LogFinalize();

  /* log without thread_id */
  LogConfig log_config3 = {0, 0, 1, 1, 0, N_LOG_ALL};
  LogInitialize(&log_config3);
  uni_snprintf(buf, sizeof(buf), "\033[0m\033[42;33m[T]\033[0m <test>%s:%d->"
         "test 10\n", function, line);
  LogWrite(N_LOG_TRACK, "test", function, line, "test %d", 10);
  H2EQ_STRCMP(buf, g_buf);
  LogFinalize();

  /* log without function_line */
  LogConfig log_config4 = {0, 0, 0, 1, 0, N_LOG_ALL};
  LogInitialize(&log_config4);
  uni_snprintf(buf, sizeof(buf), "\033[0m\033[42;33m[T]\033[0m <test>"
         "test 10\n");
  LogWrite(N_LOG_TRACK, "test", function, line, "test %d", 10);
  H2EQ_STRCMP(buf, g_buf);
  LogFinalize();
}

static void _write_log_append_feed_line(char *buf, int len, char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  _fill_customer_info(buf, len, fmt, args, 1);
  va_end(args);
}

static void _write_log_raw(char *buf, int len, char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  _fill_customer_info(buf, len, fmt, args, 0);
  va_end(args);
}

H2CASE(log, "log_overflow") {
  char buf[5] = {-1};
  int i;
  _write_log_append_feed_line(buf, sizeof(buf), "12345");
  H2EQ_MATH('1', buf[0]);
  H2EQ_MATH('2', buf[1]);
  H2EQ_MATH('3', buf[2]);
  H2EQ_MATH('\n', buf[3]);
  H2EQ_MATH('\0', buf[4]);
  memset(buf, 0xFF, sizeof(buf));
  _write_log_append_feed_line(buf, sizeof(buf) - 1, "12345");
  H2EQ_MATH('1', buf[0]);
  H2EQ_MATH('2', buf[1]);
  H2EQ_MATH('\n', buf[2]);
  H2EQ_MATH('\0', buf[3]);
  H2EQ_MATH(-1, buf[4]);
  memset(buf, 0xFF, sizeof(buf));
  _write_log_append_feed_line(buf, sizeof(buf), "12");
  H2EQ_MATH('1', buf[0]);
  H2EQ_MATH('2', buf[1]);
  H2EQ_MATH('\n', buf[2]);
  H2EQ_MATH('\0', buf[3]);
  H2EQ_MATH(-1, buf[4]);
  _write_log_raw(buf, sizeof(buf), "12345");
  H2EQ_MATH('1', buf[0]);
  H2EQ_MATH('2', buf[1]);
  H2EQ_MATH('3', buf[2]);
  H2EQ_MATH('4', buf[3]);
  H2EQ_MATH('\0', buf[4]);
  memset(buf, 0xFF, sizeof(buf));
  _write_log_raw(buf, sizeof(buf) - 1, "12345");
  H2EQ_MATH('1', buf[0]);
  H2EQ_MATH('2', buf[1]);
  H2EQ_MATH('3', buf[2]);
  H2EQ_MATH('\0', buf[3]);
  H2EQ_MATH(-1, buf[4]);
  memset(buf, 0xFF, sizeof(buf));
  _write_log_raw(buf, sizeof(buf), "12");
  H2EQ_MATH('1', buf[0]);
  H2EQ_MATH('2', buf[1]);
  H2EQ_MATH('\0', buf[2]);
  H2EQ_MATH(-1, buf[3]);
  H2EQ_MATH(-1, buf[4]);
  memset(buf, 0xFF, sizeof(buf));
  _write_log_raw(buf, sizeof(buf), "12\n");
  H2EQ_MATH('1', buf[0]);
  H2EQ_MATH('2', buf[1]);
  H2EQ_MATH('\n', buf[2]);
  H2EQ_MATH('\0', buf[3]);
  H2EQ_MATH(-1, buf[4]);
}
