#ifndef SERVICE_RESPONSE_MSG_H_
#define SERVICE_RESPONSE_MSG_H_

#ifdef __cplusplus
extern "C" {
#endif

void get_ok_msg(char *response);
void get_reject_msg(char *response);
void get_error_param_msg(char *response);
void get_error_uart_msg(char *response);

#ifdef __cplusplus
}
#endif
#endif  //  SERVICE_RESPONSE_MSG_H_
