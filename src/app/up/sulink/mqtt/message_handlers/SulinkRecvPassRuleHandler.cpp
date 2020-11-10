#include "SulinkRecvPassRuleHandler.hpp"

SulinkRecvPassRuleHandler :: SulinkRecvPassRuleHandler() : IMqttMessageHandler("sulink_recv_pass_rule_handler") {}

SulinkRecvPassRuleHandler :: ~SulinkRecvPassRuleHandler() {}

int SulinkRecvPassRuleHandler :: handle(const Message &message) {
    //TODO
    return -1;
}