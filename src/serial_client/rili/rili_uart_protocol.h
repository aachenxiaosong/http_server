#ifndef SERIAL_CLIENT_RILI_RILI_UART_CLIENT_H_
#define SERIAL_CLIENT_RILI_RILI_UART_CLIENT_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
  RILI_EVENT_CALL_LIFT = 2,
  RILI_EVENT_EXTER_VISIT = 3,
  RILI_EVENT_INTER_VISIT = 4,
  RILI_EVENT_GET_HOME = 5,
  RILI_EVENT_LIFT_STATUS = 7
} RiliUartEvent;

#define RILI_ROOM_LENGTH 4
typedef struct {
  int building_num;
  char visitor_room[RILI_ROOM_LENGTH + 1];
  char host_room[RILI_ROOM_LENGTH + 1];
} RiliRequestInterVisit;

typedef struct {
  int ack_code;
} RiliResponseInterVisit;

typedef struct {
  int building_num;
  int hall_num;
  char room[RILI_ROOM_LENGTH + 1];
} RiliRequestExterVisit;

typedef struct {
  int ack_code;
} RiliResponseExterVisit;

typedef struct {
  int building_num;
  int hall_num;
  char room[RILI_ROOM_LENGTH + 1];
} RiliRequestGetHome;

typedef struct {
  int ack_code;
} RiliResponseGetHome;

typedef struct {
  int building_num;
  char room[RILI_ROOM_LENGTH + 1];
} RiliRequestCallLift;

typedef struct {
  int ack_code;
} RiliResponseCallLift;

typedef struct {
  int building_num;
  int lift_num;
} RiliRequestLiftStatus;

typedef struct {
  int lift_num;
  int lift_status;
  int current_level;
} RiliResponseLiftStatus;

int rili_protocol_send(int event_type, void *request, void *response);

#ifdef __cplusplus
}
#endif
#endif  //  SERIAL_CLIENT_RILI_RILI_UART_CLIENT_H_

