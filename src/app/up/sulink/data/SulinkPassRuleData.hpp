#ifndef APP_UP_SULINK_DATA_SULINK_PASS_RULE_DATA_HPP_
#define APP_UP_SULINK_DATA_SULINK_PASS_RULE_DATA_HPP_

#include "SulinkPassDevice.hpp"
#include "SulinkPassRule.hpp"

class SulinkPassRuleData {
private:
    vector<SulinkPassDevice> devices;//这个设备应该是当前存在的子设备,如果设备还没注册上来,相应的规则丢弃掉;后上来的设备,用兜底的方式去拉一下
    vector<SulinkPassRule> rules;
public:
    //新增,以参数里的device为准,新增的device都要下发
    int addRule(const SulinkPassRule& rule, const vector<string>& device_codes);
    //删除,从部分device上删除,受影响的device都要下发
    int delRule(const string& rule_code, const vector<string>& device_codes);
    //更新1,删除部分人员
    int updateRule(const string& rule_code, const vector<string>& person_ids);
    //更新2,增加或更新(已存在)部分人员
    int updateRule(const string& rule_code, const vector<SulinkPassRule::Person>& persons);
    //更新3,更新时间规则(规则没有增量更新)
    int updateRule(const string& rule_code, const vector<SulinkPassRule::TimeRule>& time_rules);
};

#endif  //  APP_UP_SULINK_DATA_SULINK_PASS_RULE_DATA_HPP_
