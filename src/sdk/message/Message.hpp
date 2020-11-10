#ifndef SDK_MESSAGE_MESSAGE_HPP_
#define SDK_MESSAGE_MESSAGE_HPP_

#include <MessageType.hpp>

#define MEMBER(type, name) \
private: type m_##name; \
public: const type& name() const{return this->m_##name;}\
public: void name(type _arg) {this->m_##name=_arg;}\
private:

#define PMEMBER(type, name) \
public: type m_##name; \
public: type& name() {return this->m_##name;}\
private:

class Message {
    MEMBER(MessageType, type)
public:
    Message(MessageType type) {
        this->type(type);
    }
    virtual ~Message() {}
};

#endif  //  SDK_MESSAGE_MESSAGE_HPP_