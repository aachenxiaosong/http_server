#pragma once


enum MqTopicType {
    MQ_TOPIC_FIRST = 0,
    //lift ctrl
    MQ_TOPIC_LIFT_CTRL_BRAND_CHANGE,
    MQ_TOPIC_LIFT_CTRL_WECHAT,
    //sulink
    MQ_TOPIC_SULINK_LIFT_CTRL,
    //add your topic above MQ_TOPIC_LAST
    MQ_TOPIC_LAST
};