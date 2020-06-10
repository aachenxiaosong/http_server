#ifndef SERVICE_CALL_LIFT_H_
#define SERVICE_CALL_LIFT_H_

#ifdef __cplusplus
extern "C" {
#endif

#define CALL_LIFT_ROOM_LENGTH 4
typedef struct {
  int building_num;
  char room[CALL_LIFT_ROOM_LENGTH + 1];
} RequestCallLift;

typedef struct {
  int ack_code;
} ResponseCallLift;

int call_lift(char *request, char *response);

#ifdef __cplusplus
}
#endif
#endif  //  SERVICE_CALL_LIFT_H_
