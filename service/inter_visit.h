#ifndef SERVICE_INTER_VISIT_H_
#define SERVICE_INTER_VISIT_H_

#define INTER_VISIT_ROOM_LENGTH 4
typedef struct {
  int building_num;
  char visitor_room[INTER_VISIT_ROOM_LENGTH + 1];
  char host_room[INTER_VISIT_ROOM_LENGTH + 1];
} RequestInterVisit;

typedef struct {
  int ack_code;
} ResponseInterVisit;

int inter_visit(char *request, char *response);

#endif  //  SERVICE_INTER_VISIT_H_
