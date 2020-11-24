#ifndef _CONFIG_CONFIGURABLE_INFO_H_
#define _CONFIG_CONFIGURABLE_INFO_H_

#ifdef __cplusplus
extern "C" {
#endif


#define CLOUD_TYPE 2
/******************* common info of device *******************/
#define MY_VERSION "v3.1.0"
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
//#define MY_SERVER_URL "http://192.168.6.10:8080"
#define MY_SERVER_URL "http://101.231.106.182:8081"

/************************* urls info************************/
#if (CLOUD_TYPE == 0) //test
#define CLOUD_AUTH_URL   "http://test-route.igaicloud.cn:8088/auth/token"
#define CLOUD_HB_REPORT_URL "http://test-route.igaicloud.cn:8088/monitor/hb/up"
#define MC_REGISTER_URL_FEATURE "http://test-route.igaicloud.cn:8088/msg-center/rest/v1/client/register"
#define MC_REGISTER_URL_CP "http://test-route.igaicloud.cn:8088/ym-connect-device/api/auth/deviceClientInfo"
#elif (CLOUD_TYPE == 1) //uat
#define CLOUD_AUTH_URL   "http://uat-route.igaicloud.cn:8088/auth/token"
#define CLOUD_HB_REPORT_URL "http://uat-route.igaicloud.cn:8088/monitor/hb/up"
#define MC_REGISTER_URL_FEATURE "http://uat-route.igaicloud.cn:8088/msg-center/rest/v1/client/register"
#define MC_REGISTER_URL_CP "http://uat-route.igaicloud.cn:8088/ym-connect-device/api/auth/deviceClientInfo"
#elif (CLOUD_TYPE == 2) //product
#define CLOUD_AUTH_URL   "http://route.igaicloud.cn:8088/auth/token"
#define CLOUD_HB_REPORT_URL "http://route.igaicloud.cn:8088/monitor/hb/up"
#define MC_REGISTER_URL_FEATURE "http://route.igaicloud.cn:8088/msg-center/rest/v1/client/register"
#define MC_REGISTER_URL_CP "http://route.igaicloud.cn:8088/ym-connect-device/api/auth/deviceClientInfo"
#endif

#define CLOUD_UC_TS_URL  "http://uc.hivoice.cn/timestamp.jsp"
/***************************** sulink ***********************/
#if 1
#define SULINK_ACCESS_KEY           "847a82be3eff4060b8f4c4f3e0ed27da"
#define SULINK_SECRET_KEY           "7c3d1867d911450db784496e382dbfbb"
#define SULINK_BRAND                "xilaideng"
#define SULINK_PAYLOAD_VERSION      1
#else
#define SULINK_ACCESS_KEY           "shimao62ae1634e0b32d0"
#define SULINK_SECRET_KEY           "f6339f4974cac548080ecdeca4037489"
#define SULINK_BRAND                "yzs"
#endif
#define SULINK_SERVER               "https://test-api.shimaoiot.com/"
#define SULINK_DEVICE_REGISTER_URL  SULINK_SERVER"api/sulink/v2/device/register"
#define SULINK_HB_URL               SULINK_SERVER"device-monitor/v2/hb/up"
#define SULINK_TIME_SYNC_URL        SULINK_SERVER"api/sulink/v2/time/sync"
/************************************************************/


#define STUB_ENABLE 0

#ifdef __cplusplus
}
#endif
#endif  //  _CONFIG_CONFIGURABLE_INFO_H_
