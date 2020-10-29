#include "Dechang.hpp"
#include "SulinkSignature.hpp"
#include "SulinkDeviceRegister.hpp"
#include "SulinkTraceid.hpp"
#include "SulinkClient.hpp"
#include "SulinkMessage.hpp"
#include "SulinkHb.hpp"
#include "UniUtil.hpp"
#include <iostream>
#include <unistd.h>

using namespace std;

#define TEST_NUM 4

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
    long timestamp = UniUtil::timestampMsL();
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
        if (sulink_client.send(message) == 0) {
            break;
        }
        sleep(5);
    }
#elif (TEST_NUM == 4)
    SulinkHb hb;
    cout << "hb request result: " << hb.request() << endl;
#endif
}