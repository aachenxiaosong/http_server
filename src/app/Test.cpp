#include "Dechang.hpp"
#include <iostream>
#include "unistd.h"

using namespace std;

#define TEST_NUM 0

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
#endif
}