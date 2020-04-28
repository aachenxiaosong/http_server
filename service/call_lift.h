#ifndef SERVICE_CALL_LIFT_H_
#define SERVICE_CALL_LIFT_H_

#define CALL_LIFT_ROOM_LENGTH 4
typedef struct {
  int building_num;
  char room[CALL_LIFT_ROOM_LENGTH + 1];
} RequestCallLift;

typedef struct {
  int ack_code;
} ResponseCallLift;

int call_lift(char *request, char *response);

#endif  //  SERVICE_CALL_LIFT_H_
