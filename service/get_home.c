#include "get_home.h"
#include "response_msg.h"
#include "protocol.h"

#include "cJSON.h"
#include "log.h"

#include <stdio.h>
#include <stdlib.h>

//把回调函数也丢给event_list？然后用interruptable sleep在这等
int get_home(char *request, char *response) {
  RequestGetHome request_get_home;
  ResponseGetHome response_get_home;
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
  if (strlen(jroom->valuestring) != GET_HOME_ROOM_LENGTH) {
    LOGE("wrong length of room");
    goto L_ERROR;
  }
  request_get_home.building_num = jbuilding_num->valueint;
  request_get_home.hall_num = jhall_num->valueint;
  snprintf(request_get_home.room, sizeof(request_get_home.room), "%s", jroom->valuestring);
  LOGT("building_num:%d, hall_num:%d, room:%s",
       request_get_home.building_num, request_get_home.hall_num, request_get_home.room);
  //serial comm
  if (0 == protocol_send(EVENT_GET_HOME, &(request_get_home), &(response_get_home))) {
    if (1 == response_get_home.ack_code) {
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
