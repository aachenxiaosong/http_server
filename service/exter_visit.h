#ifndef SERVICE_EXTER_VISIT_H_
#define SERVICE_EXTER_VISIT_H_

#ifdef __cplusplus
extern "C" {
#endif

#define EXTER_VISIT_ROOM_LENGTH 4
typedef struct {
  int building_num;
  int hall_num;
  char room[EXTER_VISIT_ROOM_LENGTH + 1];
} RequestExterVisit;

typedef struct {
  int ack_code;
} ResponseExterVisit;

int exter_visit(char *request, char *response);

#ifdef __cplusplus
}
#endif
#endif  //  SERVICE_EXTER_VISIT_H_
