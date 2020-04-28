#ifndef UART_INC_PROTOCOL_H_
#define UART_INC_PROTOCOL_H_

typedef enum {
  EVENT_CALL_LIFT = 2,
  EVENT_EXTER_VISIT = 3,
  EVENT_INTER_VISIT = 4,
  EVENT_GET_HOME = 5,
  EVENT_LIFT_STATUS = 7
} UartEvent;


int protocol_send(int event_type, void *request, void *response);

#endif  //  UART_INC_PROTOCOL_H_

