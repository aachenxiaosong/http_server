//#include "Dechang.hpp"
//#include "SulinkSignature.hpp"
//#include "SulinkDeviceRegister.hpp"
#include "SulinkTraceid.hpp"
//#include "SulinkClient.hpp"
//#include "SulinkMessage.hpp"
//#include "SulinkHb.hpp"
//#include "SulinkTimeSync.hpp"
#include "SulinkLiftInitData.hpp"
#include "Mq.hpp"
//#include "UniDeviceInfo.hpp"
#include "UniUtil.hpp"
#include <iostream>
#include <unistd.h>
#include <future>
#include "UniLog.hpp"
#include "boost/foreach.hpp"
#include "boost/assign.hpp"
#include "boost/timer/timer.hpp"
#include "SlingLiftCtrl.hpp"

using namespace std;

#define TEST_NUM 11

#if 0
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
#endif
struct MqTest {
    int i;
    string s;
};

static void _mq_recv_task(void *arg)
{
    vector<MqTopicType> topic_types;
    topic_types.push_back(MQ_TOPIC_FIRST);
    topic_types.push_back(MQ_TOPIC_LAST);
    Mq mq("recv_mq", topic_types);
    MqData data = mq.recv();
    //cout << "recv msg: " << (const char *)data.content() << endl;
    MqTest *test = (MqTest *)data.content();
    cout << "recv msg: " << test->i << " " << test->s << endl;
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
    long timestamp = unisound::UniUtil::timestampMs();
    message.appKey(SULINK_ACCESS_KEY);
    message.deviceSn(unisound::UniDeviceInfo::getUdid());
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
    long timestamp = unisound::UniUtil::timestampMs();
    message.brand(SULINK_BRAND);
    message.code("12345678");//学号
    message.deviceCode(unisound::UniDeviceInfo::getUdid());
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
    //SulinkClient sulink_client;
    while (1) {
        sleep(5);
        cout << "building: " << SulinkLiftInitData::getBuildingNoBySpaceId("2094581") << endl;
        cout << "unit: " << SulinkLiftInitData::getUnitNoBySpaceId("2094581") << endl;
        cout << "home: " << SulinkLiftInitData::getHomeNoBySpaceId("2094581") << endl;
        cout << "cluster id: " << SulinkLiftInitData::getClusterIdBySpaceId("2094581") << endl;
        cout << "cluster url: " << SulinkLiftInitData::getClusterUrlBySpaceId("2094581") << endl;
        cout << "floor: " << SulinkLiftInitData::getFloorNoBySpaceId("2094581") << endl;
        cout << "device floor: " << SulinkLiftInitData::getDeviceFloorNo("hf0000123") << endl;
        cout << "device hall: " << SulinkLiftInitData::getDeviceHallNo("hf0000123") << endl;
        cout << "device work mode: " << SulinkLiftInitData::getDeviceWorkMode("hf0000123") << endl;
        cout << "device pub floor: " << SulinkLiftInitData::getDevicePubFloors("hf0000123") << endl;
        cout << "device lift id: " << SulinkLiftInitData::getDeviceLiftId("hf0000123") << endl;
    }
#elif (TEST_NUM == 8)
    cout << "before test" << endl;
    UniLog::init();
    LOGT("test_tag", "12345678901234567890123456789012345678901234567890123456789012345678901234567890");
    cout << "after test" << endl;
#elif (TEST_NUM == 9)
    auto th = async(std::launch::async, _mq_recv_task, (void *)NULL);
    sleep(1);
    Mq mq("send_mq");
    //MqData data(MQ_TOPIC_FIRST, (void *)("i am content"), strlen("i am content") + 1);
    MqTest test;
    test.i = 10;
    test.s = "test";
    MqData data(MQ_TOPIC_FIRST, (void *)(&test), sizeof(test));
    mq.send(data);
    th.wait();
#elif (TEST_NUM == 10)
    vector<int> v = (assign::list_of(1), 2, 3, 4, 5);

    BOOST_FOREACH (int x, v)
    {
        cout << x << ",";
    }
    cout << endl;
    cout << "hello world!" << endl;
#elif (TEST_NUM == 11)
    SlingLiftCtrl lift_ctrl("tcp://192.168.4.222:52000");
    SlingFloor from_floor(1);
    SlingFloor to_floor(6);
    vector<SlingFloor> to_floors;
    to_floors.push_back(to_floor);
    SlingRequestAttribute req_attr;
    req_attr.seqNum = 1;
    req_attr.clusterId = 1;
    req_attr.verificationLocation = req_attr.LOBBY;
    req_attr.verificationType = req_attr.OUTOF_CAR;
    req_attr.callAttribute = req_attr.NORMAL;
    req_attr.hallCallMode = req_attr.REGISTER_HALL;
    SlingResponse rsp_attr;
    lift_ctrl.bookElevator(from_floor, to_floor, req_attr, rsp_attr);
    sleep(5);
    lift_ctrl.bookElevator(from_floor, to_floors, req_attr, rsp_attr);
    
#endif
}
