#ifndef SDK_MESSAGE_MESSAGE_DISPATCHER_HPP_
#define SDK_MESSAGE_MESSAGE_DISPATCHER_HPP_

#include "IMessageHandler.hpp"
#include <vector>
#include <RwLock.hpp>

using namespace std;

class MessageDispatcher {
private:
    vector<IMessageHandler *> mHandlers;
    RwLock mLock;
public:
    void addMessageHandler(IMessageHandler *handler) {
        mLock.writeLock();
        mHandlers.push_back(handler);
        mLock.writeUnlock();
    }

    int onRecvMessage(const Message &message) {
        int ret = -1;
        IMessageHandler *handler;
        mLock.readLock();
        vector<IMessageHandler *> :: iterator it;
        for (it = mHandlers.begin(); it != mHandlers.end(); it++) {
            handler = *it;
            if (0 == handler->handle(message)) {
                ret = 0;
                break;
            }
        }
        mLock.readUnlock();
    }
};

#endif  //  SDK_MESSAGE_MESSAGE_DISPATCHER_HPP_