#include "exter_visit.h"
#include "response_msg.h"
#include "protocol.h"

#include "cJSON.h"
#include "log.h"

#include <stdio.h>
#include <stdlib.h>

//把回调函数也丢给event_list？然后用interruptable sleep在这等
int exter_visit(char *request, char *response) {
  RequestExterVisit request_exter_visit;
  ResponseExterVisit response_exter_visit;
  cJSON *jrequest;
  cJSON *jbuilding_num;
  cJSON *jhall_num;
  cJSON *jdata;
  cJSON *jroom;
  jrequest = cJSON_Parse(request);
  if (jrequest == NULL) {
    LOGE("parse request failed");
    goto L_ERROR;
  }
  jbuilding_num = cJSON_GetObjectItem(jrequest, "buildingNum");
  if (jbuilding_num == NULL || jbuilding_num->type != cJSON_Number) {
    LOGE("parse building num failed");
    goto L_ERROR;
  }
  jhall_num = cJSON_GetObjectItem(jrequest, "hallNum");
  if (jhall_num == NULL || jhall_num->type != cJSON_Number) {
    LOGE("parse hall num failed");
    goto L_ERROR;
  }
  jdata = cJSON_GetObjectItem(jrequest, "data");
  if (jdata == NULL) {
    LOGE("parse data failed");
    goto L_ERROR;
  }
  jroom = cJSON_GetObjectItem(jdata, "room");
  if (jroom == NULL || jroom->type != cJSON_String) {
    LOGE("parse room failed");
    goto L_ERROR;
  }
  if (strlen(jroom->valuestring) != EXTER_VISIT_ROOM_LENGTH) {
    LOGE("wrong length of room");
    goto L_ERROR;
  }
  request_exter_visit.building_num = jbuilding_num->valueint;
  request_exter_visit.hall_num = jhall_num->valueint;
  snprintf(request_exter_visit.room, sizeof(request_exter_visit.room), "%s", jroom->valuestring);
  LOGT("building_num:%d, hall_num:%d, room:%s",
       request_exter_visit.building_num, request_exter_visit.hall_num, request_exter_visit.room);
  //serial comm
  if (0 == protocol_send(EVENT_EXTER_VISIT, &(request_exter_visit), &(response_exter_visit))) {
    if (1 == response_exter_visit.ack_code) {
      get_ok_msg(response);
    } else {
      get_reject_msg(response);
    }
  } else {
    get_error_uart_msg(response);
  }
  //serial comm end
  if (NULL != jrequest) {
    cJSON_Delete(jrequest);
  }
  return 0;
L_ERROR:
  if (NULL != jrequest) {
    cJSON_Delete(jrequest);
  }
  get_error_param_msg(response);
  return -1;
}
