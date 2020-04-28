#include "lift_status.h"
#include "response_msg.h"
#include "protocol.h"

#include "cJSON.h"
#include "log.h"

#include <stdio.h>
#include <stdlib.h>


//把回调函数也丢给event_list？然后用interruptable sleep在这等
int lift_status(char *request, char *response) {
  RequestLiftStatus request_lift_status;
  ResponseLiftStatus response_lift_status;
  cJSON *jrequest;
  cJSON *jbuilding_num;
  cJSON *jdata;
  cJSON *jlift_num;
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
  jlift_num = cJSON_GetObjectItem(jdata, "liftNum");
  if (jlift_num == NULL || jlift_num->type != cJSON_Number) {
    LOGE("parse lift num failed");
    goto L_ERROR;
  }
  request_lift_status.building_num = jbuilding_num->valueint;
  request_lift_status.lift_num = jlift_num->valueint;
  LOGT("building_num:%d, lift_num:%d",
       request_lift_status.building_num, request_lift_status.lift_num);
  //serial comm
  if (0 == protocol_send(EVENT_LIFT_STATUS, &(request_lift_status), &(response_lift_status))) {
    sprintf(response, "{\"errCode\":0, \"errMsg\":\"OK\", "
                       "\"data\":{\"liftNum\":%d, \"liftStatus\":%d, \"currentLevel\":%d}}",
            response_lift_status.lift_num,
            response_lift_status.lift_status,
            response_lift_status.current_level);
  } else {
    sprintf(response, "{\"errCode\":1, \"errMsg\":\"uart error\", \"data\":null}");
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
  sprintf(response, "{\"errCode\":1, \"errMsg\":\"wrong param\", \"data\":null}");
  return -1;
}
