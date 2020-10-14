#include "ITcpReceiver.hpp"

class DechangHbReceiver : public ITcpReceiver {
private:
    void sendAck(const char *data, int len);
public:
    DechangHbReceiver();
    ~DechangHbReceiver();
    int onRecv(const char *data, int len);
    ITcpReceiver *copy();
};