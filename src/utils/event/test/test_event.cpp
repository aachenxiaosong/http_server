/*
 * All header file for c language MUST be include : in extern "C" { }
 */
extern "C" {
#include "../src/uni_event.c"
}

H2UNIT(event) {
  void setup() {}
  void teardown() {}
};

H2CASE(event, "EventCreate with null content") {
  Event *ret;
  ret = EventCreate(1, 1, NULL, NULL);
  H2EQ_TRUE(ret != NULL);
  H2EQ_MATH(1, ret->seq_id);
  H2EQ_MATH(1, ret->type);
}

H2CASE(event, "EventCreate with non-null content") {
  Event *ret;
  EventContent con = { (void*)1, (void*)2, 3, 4 };
  ret = EventCreate(1, 1, &con, NULL);
  H2EQ_TRUE(ret != NULL);
  H2EQ_MATH(((void*)1), ret->content.info);
  H2EQ_MATH(((void*)2), ret->content.extend_info);
  H2EQ_MATH(4, ret->content.err_code); 
}

H2CASE(event, "EventFree with null and non-null parameter") {
  Event *ret;
  EventContent con;
  ret = EventCreate(1, 1, &con, NULL);
  H2EQ_TRUE(ret != NULL);
  EventFree(ret);
  EventFree(NULL);
}
