#pragma once
#include "zmq.hpp"
#include "zmq_addon.hpp"
#include "MqTopicType.hpp"

#include <vector>
#include <string>

class MqData {
public:
    MqData() {
        mTopicType = MQ_TOPIC_FIRST;
        mContent = NULL;
        mSize = 0;
    }
    MqData(MqTopicType topic_type, void *content, size_t size) {
        mContent = NULL;
        this->topic_type(topic_type);
        this->content(content, size);
    }
    ~MqData() {
        if (mContent != NULL) {
            free(mContent);
        }
    }
    MqData(const MqData &data) {
        mContent = NULL;
        topic_type(data.mTopicType);
        content(data.mContent, data.mSize);
    }

    MqData& operator=(const MqData &data) {
        if (this == &data) {
            return *this;
        }
        if (mContent != NULL) {
            free(mContent);
        }
        topic_type(data.mTopicType);
        content(data.mContent, data.mSize);
        return *this;
    }
    MqTopicType topic_type() {
        return mTopicType;
    }
    void topic_type(MqTopicType type) {
        mTopicType = type;
    }
    size_t size() {
        return mSize;
    }
    void* content() {
        return mContent;
    }
    void content(void *content, size_t size) {
        if (mContent != NULL) {
            free(mContent);
        }
        mSize = size;
        mContent = malloc(mSize);
        memcpy(mContent, content, mSize);
    }
private:
    MqTopicType mTopicType;
    void *mContent;
    size_t mSize;
};

class Mq {
private:
    //publisher is shared
    static class MqContext {
        public:
        zmq::socket_t *publisher;
        zmq::context_t *context;
        MqContext();
        ~MqContext();
    } mContext;
    zmq::socket_t *mSubscriber;
    std::string mName;
public:
    Mq(const char *name);
    Mq(const char* name, std::vector<MqTopicType> sub_topics);
    ~Mq();
    MqData recv();
    void send(MqData data);
};