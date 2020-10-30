#include "Dechang.hpp"
#include "SulinkSignature.hpp"
#include "SulinkDeviceRegister.hpp"
#include "SulinkTraceid.hpp"
#include "SulinkClient.hpp"
#include "SulinkMessage.hpp"
#include "SulinkHb.hpp"
#include "SulinkTimeSync.hpp"
#include "UniUtil.hpp"
#include <iostream>
#include <unistd.h>

using namespace std;

#define TEST_NUM 7

Dechang dechang;

static void _send_add_card() {
    DechangMessageSendAddCard message;
    message.deviceId("1Z6061");
    message.cardNo(14491346);
    message.door(0);
    message.expireDate("2030-1-1-0:0:0");
    message.password("00000000");
    message.doorAccess(1);
    message.rand(0);
    message.status(0x1);
    message.userId(16);
    message.userName("xiao");
    while (0 != dechang.onRecvMessage(message.deviceId(), message)) {
        cout << "sending add card msg ----------------------------------------" << endl;
        sleep(5);
    }
}

static void _send_del_card() {
    DechangMessageSendDelCard message;
    message.deviceId("1Z6061");
    message.cardNo(14491346);
    message.userId(16);
    while (0 != dechang.onRecvMessage(message.deviceId(), message)) {
        cout << "sending del card msg ----------------------------------------" << endl;
        sleep(5);
    }
}

static void _send_del_all_card() {
    DechangMessageSendDelAllCard message;
    message.deviceId("1Z6061");
    while (0 != dechang.onRecvMessage(message.deviceId(), message)) {
        cout << "sending del all card msg ----------------------------------------" << endl;
        sleep(5);
    }
}

void AppTest() {
#if (TEST_NUM == 0)
    dechang.init();
    //_send_del_all_card();
    _send_del_card();
    _send_add_card();
#elif (TEST_NUM == 1)
    map<string, string> params;
    params["deviceCode"] = "yzs82193=#http://";
    cout << SulinkSignature::build(params, "1603420266159") << endl;
#elif (TEST_NUM == 2)
    SulinkDeviceRegister reg;
    reg.request();
    cout << reg.getMqttClientParam().toString() << endl;
#elif (TEST_NUM == 3)
    SulinkClient sulink_client;
    SulinkMessageSendDeviceInfo message;
    long timestamp = UniUtil::timestampMs();
    message.appKey(SULINK_ACCESS_KEY);
    message.deviceSn(UniUtil::deviceCode());
    message.brand(SULINK_BRAND);
    message.deviceType("边缘网关");
    message.appVersion(MY_VERSION);
    message.payloadVersion(1);
    message.timestamp(timestamp);
    message.topic("pub/device");
    message.traceId(SulinkTraceid::build(to_string(timestamp)));
    message.upTime(timestamp);
    while (1) {
        if (sulink_client.onRecvMessage(message) == 0) {
            break;
        }
        sleep(5);
    }
#elif (TEST_NUM == 4)
    SulinkHb hb;
    cout << "hb request result: " << hb.request() << endl;
#elif (TEST_NUM == 5)
    SulinkTimeSync time_sync;
    cout << "time_sync request result: " << time_sync.request() << endl;
#elif (TEST_NUM == 6)
    SulinkClient sulink_client;
    SulinkMessageSendPassRecord message;
    long timestamp = UniUtil::timestampMs();
    message.brand(SULINK_BRAND);
    message.code("12345678");//学号
    message.deviceCode(UniUtil::deviceCode());
    message.extData("");
    message.passPhoto("");
    message.passResult(0);
    message.passTime("2020-10-21 18:05:51");
    message.passType("CARD");
    message.payloadVersion(1);
    message.personId("1234567890");//人员编号
    message.personName("bob");
    message.personTemp("36.5");
    message.personType(3);
    message.reqId("1");
    message.timestamp(timestamp);
    message.topic("pub/passRule");
    message.traceId(SulinkTraceid::build(to_string(timestamp)));
    while (1) {
        if (sulink_client.onRecvMessage(message) == 0) {
            break;
        }
        sleep(5);
    }
#elif (TEST_NUM == 7)
    SulinkClient sulink_client;
    while (1) {
        sleep(5);
    }
#endif
}