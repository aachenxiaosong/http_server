/*
 * All header file for c language MUST be include : in extern "C" { }
 */
extern "C" {
#include "../src/uni_databuf.c"
}

H2UNIT(databuf) {
  void setup() {
  }
  void teardown() {
  }
};

H2CASE(databuf, "create test") {
  DataBufHandle buf_handle = NULL;
  DataBuf *databuf = NULL;
  /* normal cases */
  buf_handle = DataBufferCreate(1);
  H2EQ_TRUE(NULL != buf_handle);
  databuf = (DataBuf *)buf_handle;
  H2EQ_MATH(1 + DATABUF_FILL_BYTE_SIZE, databuf->size);
  H2EQ_MATH(databuf->head, databuf->tail);
  DataBufferDestroy(buf_handle);
  buf_handle = DataBufferCreate(1024);
  H2EQ_TRUE(NULL != buf_handle);
  databuf = (DataBuf *)buf_handle;
  H2EQ_MATH(1024 + DATABUF_FILL_BYTE_SIZE, databuf->size);
  H2EQ_MATH(databuf->head, databuf->tail);
  DataBufferDestroy(buf_handle);
  /* invalid param */
  buf_handle = DataBufferCreate(0);
  H2EQ_TRUE(NULL == buf_handle);
  buf_handle = DataBufferCreate(-1);
  H2EQ_TRUE(NULL == buf_handle);
}

H2CASE(databuf, "write test") {
  int ret;
  char src[] = "1234567890";
  DataBufHandle buf_handle;
  DataBuf *databuf;
  buf_handle = DataBufferCreate(2);
  databuf = (DataBuf *)buf_handle;
  /* return -1 when "write len" > "free len" */
  ret = DataBufferWrite(buf_handle, src, 3);
  H2EQ_MATH(-1, ret);
  H2EQ_MATH(0, databuf->head);
  /* return "write len" for normal write */
  ret = DataBufferWrite(buf_handle, src, 1);
  H2EQ_MATH(1, ret);
  H2EQ_MATH(1, databuf->head);
  H2EQ_MATH('1', databuf->buf[0]);
  /* write through buf tail */
  databuf->tail = databuf->head = 2;
  ret = DataBufferWrite(buf_handle, src, 2);
  H2EQ_MATH(2, ret);
  H2EQ_MATH(1, databuf->head);
  H2EQ_MATH('1', databuf->buf[2]);
  H2EQ_MATH('2', databuf->buf[0]);
  DataBufferDestroy(buf_handle);
}

H2CASE(databuf, "read test") {
  int ret;
  char src[] = "1234567890";
  char dst[11] = {0};
  DataBufHandle buf_handle;
  DataBuf *databuf;
  buf_handle = DataBufferCreate(10);
  databuf = (DataBuf *)buf_handle;
  DataBufferWrite(buf_handle, src, 5);
  /* return -1 when "read len" > "data len" */
  ret = DataBufferRead(dst, 6, buf_handle);
  H2EQ_MATH(-1, ret);
  H2EQ_MATH(0, databuf->tail);
  /* return "read len" for normal read */
  ret = DataBufferRead(dst, 5, buf_handle);
  H2EQ_MATH(5, ret);
  H2EQ_MATH(5, databuf->tail);
  dst[5] = '\0';
  H2EQ_STRCMP("12345", dst);
  /* read through buf tail */
  DataBufferWrite(buf_handle, src, 10);
  ret = DataBufferRead(dst, 10, buf_handle);
  H2EQ_MATH(10, ret);
  H2EQ_MATH(4, databuf->tail);
  dst[10] = '\0';
  H2EQ_STRCMP("1234567890", dst);
  DataBufferDestroy(buf_handle);
}

H2CASE(databuf, "peek test") {
  int ret;
  char src[] = "1234567890";
  char dst[11] = {0};
  DataBufHandle buf_handle;
  DataBuf *databuf;
  buf_handle = DataBufferCreate(10);
  databuf = (DataBuf *)buf_handle;
  DataBufferWrite(buf_handle, src, 5);
  ret = DataBufferPeek(dst, 5, buf_handle);
  H2EQ_MATH(5, ret);
  H2EQ_MATH(0, databuf->tail);
  dst[5] = '\0';
  H2EQ_STRCMP("12345", dst);
  ret = DataBufferPeek(dst, 5, buf_handle);
  H2EQ_MATH(5, ret);
  H2EQ_MATH(0, databuf->tail);
  dst[5] = '\0';
  H2EQ_STRCMP("12345", dst);
  DataBufferDestroy(buf_handle);
}

H2CASE(databuf, "skip test") {
  int ret;
  char src[] = "1234567890";
  char dst[11] = {0};
  DataBufHandle buf_handle;
  DataBuf *databuf;
  buf_handle = DataBufferCreate(10);
  databuf = (DataBuf *)buf_handle;
  DataBufferWrite(buf_handle, src, 5);
  DataBufferSkip(2, buf_handle);
  H2EQ_MATH(2, databuf->tail);
  DataBufferRead(dst, 3, buf_handle);
  dst[3] = '\0';
  H2EQ_STRCMP("345", dst);
  ret = DataBufferSkip(-4, buf_handle);
  H2EQ_MATH(1, databuf->tail);
  DataBufferRead(dst, 4, buf_handle);
  dst[4] = '\0';
  H2EQ_STRCMP("2345", dst);
  DataBufferWrite(buf_handle, src, 8);
  DataBufferSkip(2, buf_handle);
  H2EQ_MATH(7, databuf->tail);
  DataBufferRead(dst, 6, buf_handle);
  dst[8] = '\0';
  H2EQ_STRCMP("345678", dst);
  ret = DataBufferSkip(-7, buf_handle);
  H2EQ_MATH(6, databuf->tail);
  DataBufferRead(dst, 4, buf_handle);
  dst[4] = '\0';
  H2EQ_STRCMP("2345", dst);
  DataBufferDestroy(buf_handle);
}

H2CASE(databuf, "size test") {
  int ret;
  char src[] = "1234567890";
  char dst[11] = {0};
  DataBufHandle buf_handle;
  buf_handle = DataBufferCreate(10);
  H2EQ_MATH(0, DataBufferGetDataSize(buf_handle));
  H2EQ_MATH(10, DataBufferGetFreeSize(buf_handle));
  DataBufferWrite(buf_handle, src, 5);
  H2EQ_MATH(5, DataBufferGetDataSize(buf_handle));
  H2EQ_MATH(5, DataBufferGetFreeSize(buf_handle));
  DataBufferWrite(buf_handle, src, 5);
  H2EQ_MATH(10, DataBufferGetDataSize(buf_handle));
  H2EQ_MATH(0, DataBufferGetFreeSize(buf_handle));
}

H2CASE(databuf, "clear test") {
  int ret;
  char src[] = "1234567890";
  char dst[11] = {0};
  DataBufHandle buf_handle;
  buf_handle = DataBufferCreate(10);
  H2EQ_MATH(0, DataBufferGetDataSize(buf_handle));
  H2EQ_MATH(10, DataBufferGetFreeSize(buf_handle));
  DataBufferWrite(buf_handle, src, 5);
  DataBufferClear(buf_handle);
  H2EQ_MATH(0, DataBufferGetDataSize(buf_handle));
  H2EQ_MATH(10, DataBufferGetFreeSize(buf_handle));
  DataBufferWrite(buf_handle, src, 10);
  DataBufferClear(buf_handle);
  H2EQ_MATH(0, DataBufferGetDataSize(buf_handle));
  H2EQ_MATH(10, DataBufferGetFreeSize(buf_handle));
}
