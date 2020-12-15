#pragma once

#include <MessageType.hpp>
#include "boost/archive/text_oarchive.hpp"
#include "boost/archive/text_iarchive.hpp"
#include "boost/serialization/vector.hpp"

using namespace boost;

//for set/get
#define MEMBER(type, name) \
private: type m_##name; \
public: const type& name() const{return this->m_##name;}\
public: void name(type _arg) {this->m_##name=_arg;}\
private:
//for set/get of vector
#define PMEMBER(type, name) \
public: type m_##name; \
public: type& name() {return this->m_##name;}\
private:

//for serialization
#define SERI_BEGIN \
private: \
  friend class serialization::access; \
  template<class Archive> \
  void serialize(Archive &ar, const unsigned int version) {

#define SERI_END }

#define SERI(name) ar & m_##name;

class Message {
    MEMBER(MessageType, type)
public:
    Message(MessageType type) {
        this->type(type);
    }
    virtual ~Message() {}
};
