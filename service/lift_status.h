#ifndef SERVICE_LIFT_STATUS_H_
#define SERVICE_LIFT_STATUS_H_

typedef struct {
  int building_num;
  int lift_num;
} RequestLiftStatus;

typedef struct {
  int lift_num;
  int lift_status;
  int current_level;
} ResponseLiftStatus;

int lift_status(char *request, char *response);

#endif  //  SERVICE_LIFT_STATUS_H_
