#ifndef _CONFIGURABLE_INFO_H_
#define _CONFIGURABLE_INFO_H_

#ifdef __cplusplus
extern "C" {
#endif


#define CLOUD_TYPE 0
/******************* common info of device *******************/
#define NET_NAME  "enp2s0"
#define MY_TYPE     "ELEVATOR_IPC"
#if (CLOUD_TYPE == 0) //test
#define MY_APPKEY "c32tov4l4nehqm4364a2nct43db5swuynsgutlig"
#define MY_SECKEY "ae8142f81dc1abc5cc639f262450eb54"
#else
#define MY_APPKEY "jnilinu245equw5xy3iyid2qklz5e2266qe3zja4"
#define MY_SECKEY "bc5300620e7bc0b8b063464f7e97e04c"
#endif

/*********************** config file info**********************/
#define INIT_INFO_CONFIG_FILE "init_info.txt"

/************************* http info************************/
#define MY_SERVER_URL "http://192.168.6.10:8080"
//#define MY_SERVER_URL "http://101.231.106.182:8081"

/************************* urls info************************/
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

#define STUB_ENABLE 0

#ifdef __cplusplus
}
#endif
#endif  //  _CONFIGURABLE_INFO_H_
