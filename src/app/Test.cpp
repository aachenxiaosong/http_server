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

#define TEST_NUM 6

Dechang dechang;

static void _send_add_card() {
    DechangMessageSendAddCard message;
    message.device_id("1Z6061");
    message.card_no(14491346);
    message.door(0);
    message.expire_date("2030-1-1-0:0:0");
    message.password("00000000");
    message.door_access(1);
    message.rand(0);
    message.status(0x1);
    message.user_id(16);
    message.user_name("xiao");
    while (0 != dechang.onRecvMessage(message.device_id(), message)) {
        cout << "sending add card msg ----------------------------------------" << endl;
        sleep(5);
    }
}

static void _send_del_card() {
    DechangMessageSendDelCard message;
    message.device_id("1Z6061");
    message.card_no(14491346);
    message.user_id(16);
    while (0 != dechang.onRecvMessage(message.device_id(), message)) {
        cout << "sending del card msg ----------------------------------------" << endl;
        sleep(5);
    }
}

static void _send_del_all_card() {
    DechangMessageSendDelAllCard message;
    message.device_id("1Z6061");
    while (0 != dechang.onRecvMessage(message.device_id(), message)) {
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
    cout << reg.toString() << endl;
#elif (TEST_NUM == 3)
    SulinkClient sulink_client;
    SulinkMessageSendDeviceInfo message;
    long timestamp = UniUtil::timestampMs();
    message.app_key(SULINK_ACCESS_KEY);
    message.device_sn(UniUtil::deviceCode());
    message.brand(SULINK_BRAND);
    message.device_type("边缘网关");
    message.app_version(MY_VERSION);
    message.payload_version(1);
    message.timestamp(timestamp);
    message.topic("pub/device");
    message.trace_id(SulinkTraceid::build(to_string(timestamp)));
    message.up_time(timestamp);
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
    message.device_code(UniUtil::deviceCode());
    message.ext_data("");
    message.pass_photo("");
    message.pass_result(0);
    message.pass_time("2020-10-21 18:05:51");
    message.pass_type("CARD");
    message.payload_version(1);
    message.person_id("1234567890");//人员编号
    message.person_name("bob");
    message.person_temp("36.5");
    message.person_type(3);
    message.req_id("1");
    message.timestamp(timestamp);
    message.topic("pub/passRule");
    message.trace_id(SulinkTraceid::build(to_string(timestamp)));
    while (1) {
        if (sulink_client.onRecvMessage(message) == 0) {
            break;
        }
        sleep(5);
    }
#endif
}