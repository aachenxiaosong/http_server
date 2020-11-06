#ifndef APP_UP_SULINK_MQTT_DATA_SULINK_PASS_RULE_HPP_
#define APP_UP_SULINK_MQTT_DATA_SULINK_PASS_RULE_HPP_

#include <string>
#include <vector>

using namespace std;

struct SulinkPassRule {
    struct Card { // 人员持有卡号
        string num;
        string type; // 1-身份证 2-护照 3-军人证 4-驾照 5-港澳通行证 6-台胞证 7-其他 8-IC卡
    };
    struct Face { // 人脸id
        long faceId;
    };
    struct Floor { // 人员楼层全向
        string floors;
        string defaultFloor;
        long spaceId;
    };
    struct Password { // 人员密码
        string password;
        int type; //  1-正向密码  2-逆向密码
    };
    struct Finger {
        string id;
        int type; // 1-通行指纹 2-报警指纹
        int order; // 指纹排序
    };
    struct Person {
        long timestamp; // 更新时间戳
        string id;
        string code; // 定义的人员编号,
        string name;
        int type; // 0-学生 1-宿管 2-老师 3-业主 4-租客 5-物管 6-经理 7-客人 8-游客 99-陌生人
        string imageUrl;
        vector<Card> cards;
        vector<Face> faces;
        vector<Floor> floors;
        vector<Password> passwords;
        vector<Finger> fingers;
        Person(const Person& p) {

        }
    };
    struct TimeRule {
        long timestamp; // 更新时间戳
        long id;
        string name;
        int type; // 0-自定义 1-长期
        string dayBeginTime; // timeRuleType：0有效,格式:yyyy-MM-dd hh:mm
        string dayEndTime; // timeRuleType：0有效,格式:yyyy-MM-dd hh:mm
        string effectiveWeek; // 生效星期(例如：周一到周五能通行， 那么取值是 12345)
        string effectiveBeginTime; // 开始时段格式:hh:mm)
        string effectiveEndTime; // 结束时段(格式:hh:mm)
        int permitType; // 通行许可（0:不允许 1:允许）
    };
    
    long timestamp; // 更新时间
    string ruleCode;
    string ruleName;
    string recognizeType; // FACE-人脸 CARD-卡 QRCODE-二维码 PWD-密码 FINGER-指纹，多种类型以，号分割
    vector<string> devices; //device code,用来快速索引需要下发的设备
    vector<Person> persons;
    vector<TimeRule> timeRules;
};
#endif  //  APP_UP_SULINK_MQTT_DATA_SULINK_PASS_RULE_HPP_
