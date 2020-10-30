#ifndef APP_UP_SULINK_DATA_SULINK_PASS_RULE_DATA_HPP_
#define APP_UP_SULINK_DATA_SULINK_PASS_RULE_DATA_HPP_

#include "SulinkPassRule.hpp"

class SulinkPassRuleData {
private:
    vector<SulinkPassRule> rules;
public:
    int addRule(cosnt SulinkPassRule& rule);
    int delRule(const string& rule_code);
};

#endif  //  APP_UP_SULINK_DATA_SULINK_PASS_RULE_DATA_HPP_
