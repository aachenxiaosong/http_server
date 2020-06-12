/*
 * All header file for c language MUST be include : in extern "C" { }
 */
extern "C" {
#include "../src/uni_fsm.c"
}

enum {
  S1,
  S2,
  S3
};

enum {
  E1,
  E2,
  E3
};

static Result h1(void *args);
static Result h2(void *args);
static Result h3(void *args);

static MicroFsmTransition g_trans[] = { 
  {S1, E1, h1},
  {S2, E2, h2},
  {S3, E3, h3},
};
static MicroFsmStruct g_fsm;

static Result h1(void *args) {
  FsmSetState(&g_fsm, S2);
  return E_HOLD;
}

static Result h2(void *args) {
  FsmSetState(&g_fsm, S3);
  return E_FAILED;
}

static Result h3(void *args) {
  FsmSetState(&g_fsm, S1);
  return E_OK;
}

H2UNIT(fsm) {
  void setup() {
    FsmInit(&g_fsm, "fsm_test", g_trans, sizeof(g_trans) / sizeof(g_trans[0]));
    FsmSetState(&g_fsm, S1);
  }
  void teardown() {
  }
};

H2CASE(fsm, "set/get test") {
  H2EQ_MATH(S1, FsmGetState(&g_fsm));
  FsmSetState(&g_fsm, S2);
  H2EQ_MATH(S2, FsmGetState(&g_fsm));
  FsmSetState(&g_fsm, S3);
  H2EQ_MATH(S3, FsmGetState(&g_fsm));
}

H2CASE(fsm, "validation test") {
  FsmSetState(&g_fsm, S1);
  H2EQ_MATH(E_OK, IsValidEventId(&g_fsm, E1));
  H2EQ_MATH(E_REJECT, IsValidEventId(&g_fsm, E2));
  FsmSetState(&g_fsm, S2);
  H2EQ_MATH(E_OK, IsValidEventId(&g_fsm, E2));
  H2EQ_MATH(E_REJECT, IsValidEventId(&g_fsm, E3));
  FsmSetState(&g_fsm, S3);
  H2EQ_MATH(E_OK, IsValidEventId(&g_fsm, E3));
  H2EQ_MATH(E_REJECT, IsValidEventId(&g_fsm, E1));
}

H2CASE(fsm, "process test") {
  FsmSetState(&g_fsm, S1);
  H2EQ_MATH(E_REJECT, ProcessNextEvent(&g_fsm, NULL, E2));
  H2EQ_MATH(E_HOLD, ProcessNextEvent(&g_fsm, NULL, E1));
  H2EQ_MATH(S2, FsmGetState(&g_fsm));
  H2EQ_MATH(E_REJECT, ProcessNextEvent(&g_fsm, NULL, E3));
  H2EQ_MATH(E_FAILED, ProcessNextEvent(&g_fsm, NULL, E2));
  H2EQ_MATH(S3, FsmGetState(&g_fsm));
  H2EQ_MATH(E_REJECT, ProcessNextEvent(&g_fsm, NULL, E1));
  H2EQ_MATH(E_OK, ProcessNextEvent(&g_fsm, NULL, E3));
  H2EQ_MATH(S1, FsmGetState(&g_fsm));
}
