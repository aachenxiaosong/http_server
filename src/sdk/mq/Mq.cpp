#include "Mq.hpp"
#include "UniLog.hpp"

#define MQ_ADDRESS "inproc://unisound"

#define MQ_TAG mName.c_str()

using namespace std;
using namespace zmq;

Mq :: MqContext :: MqContext() {
    context = new context_t();
    publisher = new socket_t(*context, socket_type::pub);
    publisher->bind(MQ_ADDRESS);
}

Mq :: MqContext :: ~MqContext() {
    delete publisher;
    delete context;
}

Mq :: MqContext Mq :: mContext;

Mq :: Mq(const char* name) {
    //just send, no recv
    mName = name;
    mSubscriber = NULL;
}

Mq :: Mq(const char* name, vector<MqTopicType> sub_topics) {
    mName = name;
    mSubscriber = new socket_t(*mContext.context, socket_type::sub);
    mSubscriber->connect(MQ_ADDRESS);
    for (auto topic : sub_topics) {
        mSubscriber->set(sockopt::subscribe, to_string(topic));
    }
}

Mq :: ~Mq() {
    delete mSubscriber;
}

MqData Mq :: recv() {
    std::vector<zmq::message_t> recv_msgs;
    recv_result_t result = recv_multipart(*mSubscriber, std::back_inserter(recv_msgs));
    string topic((const char*)recv_msgs[0].data());
    LOGT(MQ_TAG, "recving message from topic " + topic);
    return MqData((MqTopicType)atoi(topic.c_str()), recv_msgs[1].data(), recv_msgs[1].size());
}

void Mq :: send(MqData data) {
    string topic = to_string(data.topic_type());
    mContext.publisher->send(const_buffer(topic.data(), topic.length() + 1), zmq::send_flags::sndmore);
    mContext.publisher->send(const_buffer(data.content(), data.size()));
    LOGT(MQ_TAG, "sending message to topic " + topic);
}