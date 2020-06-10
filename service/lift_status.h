#ifndef SERVICE_LIFT_STATUS_H_
#define SERVICE_LIFT_STATUS_H_

#ifdef __cplusplus
extern "C" {
#endif

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

#ifdef __cplusplus
}
#endif
#endif  //  SERVICE_LIFT_STATUS_H_
