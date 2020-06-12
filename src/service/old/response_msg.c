#include "response_msg.h"

#include <stdio.h>
#include <stdlib.h>

static const char *ok_msg = "{\"errCode\":0, \"errMsg\":\"OK\", \"ackCode\":1}";
static const char *reject_msg = "{\"errCode\":0, \"errMsg\":\"OK\", \"ackCode\":0}";
static const char *error_param_msg = "{\"errCode\":1, \"errMsg\":\"wrong param\", \"ackCode\":0}";
static const char *error_uart_msg = "{\"errCode\":2, \"errMsg\":\"uart error\", \"ackCode\":0}";

void get_ok_msg(char *response) {
  sprintf(response, "%s", ok_msg);
}

void get_reject_msg(char *response) {
  sprintf(response, "%s", reject_msg);
}

void get_error_param_msg(char *response) {
  sprintf(response, "%s", error_param_msg);
}

void get_error_uart_msg(char *response) {
  sprintf(response, "%s", error_uart_msg);
}
