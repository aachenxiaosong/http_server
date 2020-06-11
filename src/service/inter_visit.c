#include "inter_visit.h"
#include "response_msg.h"
#include "protocol.h"

#include "cJSON.h"
#include "log.h"

#include <stdio.h>
#include <stdlib.h>

//把回调函数也丢给event_list？然后用interruptable sleep在这等
int inter_visit(char *request, char *response) {
  RequestInterVisit request_inter_visit;
  ResponseInterVisit response_inter_visit;
  cJSON *jrequest;
  cJSON *jbuilding_num;
  cJSON *jdata;
  cJSON *jvisitor_room;
  cJSON *jhost_room;
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
  jdata = cJSON_GetObjectItem(jrequest, "data");
  if (jdata == NULL) {
    LOGE("parse data failed");
    goto L_ERROR;
  }
  jvisitor_room = cJSON_GetObjectItem(jdata, "visitorRoom");
  if (jvisitor_room == NULL || jvisitor_room->type != cJSON_String) {
    LOGE("parse visitor_room failed");
    goto L_ERROR;
  }
  if (strlen(jvisitor_room->valuestring) != INTER_VISIT_ROOM_LENGTH) {
    LOGE("wrong length of visitor_room");
    goto L_ERROR;
  }
  jhost_room = cJSON_GetObjectItem(jdata, "hostRoom");
  if (jhost_room == NULL || jhost_room->type != cJSON_String) {
    LOGE("parse host_room failed");
    goto L_ERROR;
  }
  if (strlen(jhost_room->valuestring) != INTER_VISIT_ROOM_LENGTH) {
    LOGE("wrong length of host_room");
    goto L_ERROR;
  }
  request_inter_visit.building_num = jbuilding_num->valueint;
  snprintf(request_inter_visit.visitor_room, sizeof(request_inter_visit.visitor_room), "%s", jvisitor_room->valuestring);
  snprintf(request_inter_visit.host_room, sizeof(request_inter_visit.host_room), "%s", jhost_room->valuestring);
  LOGT("building_num:%d, visitor_room:%s, host_room:%s",
       request_inter_visit.building_num, request_inter_visit.visitor_room, request_inter_visit.host_room);
  //serial comm
  if (0 == protocol_send(EVENT_INTER_VISIT, &(request_inter_visit), &(response_inter_visit))) {
    if (1 == response_inter_visit.ack_code) {
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
