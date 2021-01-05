#include<stdlib.h>
#include<stdio.h>
#include<errno.h>
#include<string.h>
#include<netdb.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<unistd.h>
#include<arpa/inet.h>

#define DEF_PORT    8080
#define DEF_NO_ACK  5
#define DEF_ERR_SUM 5
#define UART_LENGTH_MAX 256

typedef struct {
  unsigned char   recv_cache[UART_LENGTH_MAX];
  int             recv_cache_index;
  int             recv_timeout;
} Serial;

static Serial g_serial;

static unsigned char _calc_sum(unsigned char *data, int len)
{
    char sum = 0;
    int i = 0;
    for(i = 0; i < len; i++) {
         sum += data[i];
     }
    sum = ~sum;
    sum += 1;

    return sum;
}

static int _deal_with_recv_byte(unsigned char recv_byte) {
  int ret = -1;

  if (g_serial.recv_cache_index == 0) {
    if (recv_byte == 0x02) { //head 
      g_serial.recv_cache[g_serial.recv_cache_index] = recv_byte;
      g_serial.recv_cache_index++;
     }
  } else if (g_serial.recv_cache_index == 1) {
    if (recv_byte == 0x00) { 
      g_serial.recv_cache[g_serial.recv_cache_index] = recv_byte;
      g_serial.recv_cache_index++;
    } else {
      printf("serial recv byte failed, index = %d\n", g_serial.recv_cache_index);
      g_serial.recv_cache_index = 0;
    }
  } else if (g_serial.recv_cache_index == 2) {
    if (recv_byte == 0x01) { 
      g_serial.recv_cache[g_serial.recv_cache_index] = recv_byte;
      g_serial.recv_cache_index++;
    } else {
      printf("serial recv byte failed, index = %d\n", g_serial.recv_cache_index);
      g_serial.recv_cache_index = 0;
    }
  } else if (g_serial.recv_cache_index == 3) {
    if (recv_byte == 0x06) { 
      g_serial.recv_cache[g_serial.recv_cache_index] = recv_byte;
      g_serial.recv_cache_index++;
    } else {
      printf("serial recv byte failed, index = %d\n", g_serial.recv_cache_index);
      g_serial.recv_cache_index = 0;
    }
  } else if (g_serial.recv_cache_index == 4) {
    if (recv_byte == 0x01) { 
      g_serial.recv_cache[g_serial.recv_cache_index] = recv_byte;
      g_serial.recv_cache_index++;
    } else {
      printf("serial recv byte failed, index = %d\n", g_serial.recv_cache_index);
      g_serial.recv_cache_index = 0;
    }
  } else if (g_serial.recv_cache_index == 5) {
    if (recv_byte == 0x00) { 
      g_serial.recv_cache[g_serial.recv_cache_index] = recv_byte;
      g_serial.recv_cache_index++;
    } else {
      printf("serial recv byte failed, index = %d\n", g_serial.recv_cache_index);
      g_serial.recv_cache_index = 0;
    }
  } else if (g_serial.recv_cache_index == 11) {
    if (recv_byte == 0x03) { //tail
      g_serial.recv_cache[g_serial.recv_cache_index] = recv_byte;
      g_serial.recv_cache_index++;
      if(_calc_sum(&g_serial.recv_cache[1], 9) != g_serial.recv_cache[10]) {
        printf("serial sum failed, index = %d\n", g_serial.recv_cache_index);
        g_serial.recv_cache_index = 0;
      } else {
        printf("serial recv frame succ\n");
        ret = 0;
        }
    } else {
      printf("serial recv byte failed, index = %d\n", g_serial.recv_cache_index);
    }
   g_serial.recv_cache_index = 0;
  } else {
    g_serial.recv_cache[g_serial.recv_cache_index] = recv_byte;
    g_serial.recv_cache_index++;
  }
  
  return ret;
}


int main(int argc ,char *argv[])
{
    int sockfd,new_fd;
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    int sin_size,iDataNum,iLen;
    int portnumber, noAckNum, errSumNum, recvNum;
    unsigned char buffer[4096];
     

    if(argc < 2) {
        fprintf(stderr,"Usage:%s port [no ack] [err sum]\a\n", argv[0]);
        return 0;
     }
    portnumber = atoi(argv[1]);
    if(portnumber <= 0)
     {
        portnumber = DEF_PORT;
     }

    noAckNum = 0;
    if(argc > 2)
     {
        noAckNum = atoi(argv[2]);
        if(noAckNum <= 0)
          {
            noAckNum = DEF_NO_ACK;
          }
     }

    errSumNum = 0;
    if(argc > 3)
     {
        errSumNum = atoi(argv[3]);
        if(errSumNum <= 0)
          {
            errSumNum = DEF_ERR_SUM;
          }
     }
    printf("setting, no ack:%d err sum:%d\n", noAckNum, errSumNum);
    recvNum = 0;

    printf("create socket, port:%d\n", portnumber);
    if((sockfd=socket(AF_INET,SOCK_STREAM,0))==-1)
     {
        fprintf(stderr,"Socket error:%s\n\a",strerror(errno));
        return 0;
     }

    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &(int){1},
                 sizeof(int)) != 0) {  
        fprintf(stderr,"Socket opt reuseaddr:%s\n\a",strerror(errno));
        close(sockfd);
        sockfd = -1;
        return 0;
     }
    bzero(&server_addr,sizeof(struct sockaddr_in));
    server_addr.sin_family=AF_INET;
    server_addr.sin_addr.s_addr=htonl(INADDR_ANY);
    server_addr.sin_port=htons(portnumber);
    if(bind(sockfd,(struct sockaddr *)(&server_addr),sizeof(struct sockaddr))==-1)
     {
        fprintf(stderr,"Bind error:%s\n\a",strerror(errno));
        return 0;
     }
    if(listen(sockfd,5)==-1)
     {
        fprintf(stderr,"Listen error:%s\n\a",strerror(errno));
        return 0;
     }
    while(1)
     {
       char iSdata[10] = {0x2, 0x80, 0x3, 0x3, 0x91, 0xff, 0x0, 0xea, 0x3, 0x0};
       sin_size=sizeof(struct sockaddr_in);
       printf("enter accept....\n");
       if((new_fd=accept(sockfd,(struct sockaddr *)(&client_addr),&sin_size))==-1)
         {
            fprintf(stderr,"Accept error:%s\n\a",strerror(errno));
            return 0;
         }
       fprintf(stdout,"Server get connection from %s\n",
              inet_ntoa(client_addr.sin_addr));
         
       
       while(1) 
         {
           iLen = 0;
           iDataNum=recv(new_fd,buffer,4096,0);
           if(iDataNum<=0)
              { 
              perror("Recv\n");
              break;
              }
           printf("\nRecv data is:\n");
           for(iLen = 0; iLen < iDataNum; iLen++)
              {
               printf("0x%02x ", buffer[iLen]);
               if(iLen && (iLen + 1) % 16 == 0)
                  printf("\n");
              }
           printf("\n");

           for(iLen = 0; iLen < iDataNum; iLen++)
              {
               if(_deal_with_recv_byte(buffer[iLen]) == 0) 
                   {
                   printf("ack to client\n");
                   recvNum++;
                   if(noAckNum && recvNum % noAckNum == 0) 
                         {
                       printf("test no ack to client\n");
                       continue;
                         }
                   if(errSumNum && recvNum % errSumNum == 0) 
                         {
                       printf("test err sum ack to client\n");
                       send(new_fd, "\x02\x80", 9, 0);
                       continue;
                         }
                   send(new_fd, iSdata, 9, 0);
                   }
              }
           
         }
       close(new_fd);
       new_fd = -1;
     }
    close(sockfd);
    return 0;
}
