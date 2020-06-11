#ifndef SERVICE_GET_HOME_H_
#define SERVICE_GET_HOME_H_

#ifdef __cplusplus
extern "C" {
#endif

#define GET_HOME_ROOM_LENGTH 4

typedef struct {
  int building_num;
  int hall_num;
  char room[GET_HOME_ROOM_LENGTH + 1];
} RequestGetHome;

typedef struct {
  int ack_code;
} ResponseGetHome;

int get_home(char *request, char *response);

#ifdef __cplusplus
}
#endif
#endif  //  SERVICE_GET_HOME_H_
