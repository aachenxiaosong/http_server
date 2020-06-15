#ifndef _CONFIGURABLE_INFO_H_
#define _CONFIGURABLE_INFO_H_

#ifdef __cplusplus
extern "C" {
#endif

/******************* common info of device *******************/
#define NET_NAME "enp2s0"

/************************ device info***********************/
#define MY_APPKEY "gkmgvhnht2gcb4m35vy6emffztfdxdlirtkhhhin"
#define MY_SECKEY "efecfa5a4f75a915140c526e8201832c"

/************************* urls info************************/
#define CLOUD_TYPE 0
#if (CLOUD_TYPE == 0) //test
#define CLOUD_AUTH_URL   "http://test-route.igaicloud.cn:8088/auth/token"
#define MC_REGISTER_URL_FEATURE "http://test-route.igaicloud.cn:8088/msg-center/rest/v1/client/register"
#define MC_REGISTER_URL_CP "http://test-route.igaicloud.cn:8088/ym-connect-device/api/auth/deviceClientInfo"
#elif (CLOUD_TYPE == 1) //uat
#define CLOUD_AUTH_URL   "http://uat-route.igaicloud.cn:8088/auth/token"
#define MC_REGISTER_URL_FEATURE "http://uat-route.igaicloud.cn:8088/msg-center/rest/v1/client/register"
#define MC_REGISTER_URL_CP "http://uat-route.igaicloud.cn:8088/ym-connect-device/api/auth/deviceClientInfo"
#elif (CLOUD_TYPE == 2) //product
#define CLOUD_AUTH_URL   "http://route.igaicloud.cn:8088/auth/token"
#define MC_REGISTER_URL_FEATURE "http://route.igaicloud.cn:8088/msg-center/rest/v1/client/register"
#define MC_REGISTER_URL_CP "http://route.igaicloud.cn:8088/ym-connect-device/api/auth/deviceClientInfo"
#endif

#define CLOUD_UC_TS_URL  "http://uc.hivoice.cn/timestamp.jsp"

#ifdef __cplusplus
}
#endif
#endif  //  _CONFIGURABLE_INFO_H_
