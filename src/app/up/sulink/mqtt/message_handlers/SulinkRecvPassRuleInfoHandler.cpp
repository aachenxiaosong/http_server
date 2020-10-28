#include "SulinkRecvPassRuleInfoHandler.hpp"

SulinkRecvPassRuleInfoHandler :: SulinkRecvPassRuleInfoHandler() : IMqttMessageHandler("sulink_recv_pass_rule_handler") {}

SulinkRecvPassRuleInfoHandler :: ~SulinkRecvPassRuleInfoHandler() {}

int SulinkRecvPassRuleInfoHandler :: handle(const Message &message) {
    //TODO
    return 0;
}