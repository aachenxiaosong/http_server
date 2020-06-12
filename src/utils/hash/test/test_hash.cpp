/*
 * All header file for c language MUST be include : in extern "C" { }
 */
extern "C" {
#include "../src/bkdr_hash.c"
}

H2UNIT(hash) {
};

static char *s1 = "hello";
static char *s2 = "hello world";
static char *s3 = "1234567890fdshafjldsjakfpheuwioqpfndjskalfndjkwlfnejsakflndsjkalfnejiwaphgduspanlfdslafnjiepqwnfdjkslanfjkesnfeiwanfdjslanfjeklwnfdjskalfnnjds;anfjkdsanfjkdlsnafdlsanfjdkslafnjiepwnqfjekd;sanfdjsi[apfy89  -3r789ea-fjdksla' d,.v dkopfjei0wq[jtkelw'fmdsaklfjiew[qjfkelw;fmkdsla[cm,x.z/cjklfd;af.m,d.jklfd;sajifld;jsakfld;safn;fn3q;r38498t0rufd89s0fud8afjaR #@RTRQEFWFR R#@@!$#%!";
static char *s4 = "";

H2CASE(hash, "bkdr hash test") {
  uni_u32 result;
  result = bkdr_hash(s1);
  H2EQ_TRUE(result > 0);
  result = bkdr_hash(s2);
  H2EQ_TRUE(result > 0);
  result = bkdr_hash(s3);
  H2EQ_TRUE(result > 0);
  result = bkdr_hash(s4);
  H2EQ_TRUE(result == 0);
  result = bkdr_hash(NULL);
  H2EQ_TRUE(result == 0);
  H2EQ_TRUE(bkdr_hash(s3) == bkdr_hash(s3));
  H2EQ_TRUE(bkdr_hash(s2) != bkdr_hash(s3));
}
