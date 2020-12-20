#include "SlingLiftCtrl.hpp"
#include "Poco/URI.h"
#include "UniLog.hpp"
#include "uni_iot.h"

#define SLING_LIFT_CTRL_TAG "sling_lift_ctrl"

SlingLiftCtrl :: SlingLiftCtrl(const string& url)
{
    Poco::URI uri(url);
    mIp = uri.getHost();
    mPort = uri.getPort();
}

int SlingLiftCtrl :: bookElevator(const string& from_floor, const string& to_floor)
{
    //only front door
    unsigned char request[12] = {0x02, 0x00, 0x01, 0x06, 0x01, 0x00, 0x90, 0xFF, 0x00, 0xFF, 0xFF, 0x03};
    request[7] = (unsigned char)atoi(from_floor.c_str());
    request[9] = (unsigned char)atoi(to_floor.c_str());
    request[10] = (~(request[1] + request[2] + request[3] + request[4] + request[5] + request[6] + request[7] + request[8] + request[9])) + 1;
    unsigned char response[10] = {0};
    int response_len = 10;
    LOGT(SLING_LIFT_CTRL_TAG, "book elevator request: 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x",
         request[0], request[1], request[2], request[3], request[4], request[5], request[6], request[7], request[8], request[9], request[10], request[11]);
    if (0 != tcpSend(request, sizeof(request), response, &response_len)) {
        LOGE(SLING_LIFT_CTRL_TAG, "book elevator request is sent failed");
        return -1;
    }
    if (response_len < 9) {
        LOGE(SLING_LIFT_CTRL_TAG, "book elevator response is too short(%d bytes, 9 bytes expected)", response_len);
        return -1;
    }
    LOGT(SLING_LIFT_CTRL_TAG, "book elevator response: 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x",
         response[0], response[1], response[2], response[3], response[4], response[5], response[6], response[7], response[8]);
    unsigned char expected_response[] = {0x02, 0x80, 0x03, 0x03, 0x91, 0xFF, 0x00, 0xEA, 0x03};
    for (int i = 0; i < sizeof(expected_response); i++) {
        if (response[i] != expected_response[i]) {
            LOGE(SLING_LIFT_CTRL_TAG, "book elevator response does not match");
            return -1;
        }
    }
    return 0;
}

int SlingLiftCtrl :: tcpSend(unsigned char *request, int request_len, unsigned char *response, int *response_len) {
  int sockfd;
  struct sockaddr_in server_addr;

  if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1)
  {
    LOGE(SLING_LIFT_CTRL_TAG, "get socket failed");
    return -1;
  }
  struct timeval timeout = {3, 0}; //3s
  setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, (const char*)&timeout, sizeof(timeout));
  setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout));
  server_addr.sin_family = PF_INET;
  server_addr.sin_port = htons(mPort);
  server_addr.sin_addr.s_addr = inet_addr(mIp.c_str());
  if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr))  == -1)
  {
    LOGE(SLING_LIFT_CTRL_TAG, "connect failed");
    return -1;
  }
  send(sockfd, request, request_len, 0);
  if ((*response_len = recv(sockfd, response, *response_len, 0)) == -1)
  {
    LOGE(SLING_LIFT_CTRL_TAG, "recv failed");
    return -1;
  }
  close(sockfd);
  return 0;
}
