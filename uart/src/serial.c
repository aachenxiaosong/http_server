#include "serial.h"

#include "cJSON.h"
#include "list_head.h"
#include "log.h"

#include <pthread.h>

#define LIST_MAX_NODE   64
#define UART_LENGTH_MAX 256
#define FRAME_LEN_MAX   32

unsigned char g_request_id;
typedef struct {
  list_head     link;
  int           size;
  unsigned char data[FRAME_LEN_MAX];
} node_t;

typedef struct {
  int             uart_fd;
  unsigned char   recv_cache[UART_LENGTH_MAX];
  int             recv_cache_index;
  list_head       send_list;
  pthread_mutex_t send_list_mutex;
  list_head       recv_list;
  pthread_mutex_t recv_list_mutex;
} Serial;

static Serial g_serial;

static int _is_send_list_empty() {
  int empty;
  pthread_mutex_lock(&g_serial.send_list_mutex);
  empty = list_empty(&g_serial.send_list);
  pthread_mutex_unlock(&g_serial.send_list_mutex);
  return empty;
}

static int _is_recv_list_empty() {
  int empty;
  pthread_mutex_lock(&g_serial.recv_list_mutex);
  empty = list_empty(&g_serial.recv_list);
  pthread_mutex_unlock(&g_serial.recv_list_mutex);
  return empty;
}

int _push_into_list(unsigned char *buf, int n, list_head *list,
                    pthread_mutex_t *mutex) {
  int i = 0;
  node_t *node;
  node = (node_t *)malloc(sizeof(node_t));
  memset(node, 0x0, sizeof(node_t));
  for (i = 0; i < n; i++) {
    node->data[i] = buf[i];
  }
  node->size = n;
  LOGT("push into %s list", list == &g_serial.send_list ? "send" : "recv");
  for (i = 0; i < n; i++) {
    printf(" %02x ", buf[i]);
  }
  printf("\n");
  pthread_mutex_lock(mutex);
  list_add(&node->link, list); 
  pthread_mutex_unlock(mutex);
  return 0;
}

int _pull_from_list(unsigned char *buf, int *n, list_head *list,
                    pthread_mutex_t *mutex) {
  int i = 0;
  node_t *node;
  pthread_mutex_lock(mutex);
  node = list_get_head_entry(list, node_t, link);
  if (node) {
    list_del(&node->link);
  }
  pthread_mutex_unlock(mutex);
  if (node == NULL) {
    return -1;
  }
  for (i = 0; i < node->size; i++) {
    buf[i] = node->data[i];
  }
  *n = node->size;
  LOGT("pull from %s list", list == &g_serial.send_list ? "send" : "recv");
  for (i = 0; i < *n; i++) {
    printf(" %02x ", buf[i]);
  }
  printf("\n");
  return 0;
}

// get last data in receive link list
int serial_recv(unsigned char* buf, int *n) {
  if (1) {
  return _pull_from_list(buf, n, &g_serial.recv_list, &g_serial.recv_list_mutex);
  } else {
    static int i = 0;
    memset(buf, 0, 10);
    if (i % 2 == 0) {
    buf[0] = 0x02;
    buf[1] = g_request_id;
    buf[2] = 3;
    buf[3] = 4;
    buf[7] = 5;
    buf[8] = buf[1] + buf[2] + buf[3] + buf[7];
    buf[9] = 0x03;
    } else {
    buf[0] = 0x02;
    buf[1] = g_request_id;
    buf[2] = 1;
    buf[8] = buf[1] + buf[2];
    buf[9] = 0x03;
    }
    i++;
    return 0;
  }
}

static void _deal_with_recv_byte(char recv_byte) {
  if (g_serial.recv_cache_index == 0) {
    if (recv_byte == 0x02) { //head 1
      g_serial.recv_cache[g_serial.recv_cache_index] = recv_byte;
      g_serial.recv_cache_index++;
    }
  } else if (g_serial.recv_cache_index == 9) {
    if (recv_byte == 0x03) { //tail
      g_serial.recv_cache[g_serial.recv_cache_index] = recv_byte;
      g_serial.recv_cache_index++;
      _push_into_list(g_serial.recv_cache, g_serial.recv_cache_index,
                      &g_serial.recv_list, &g_serial.recv_list_mutex);
    } else {
      LOGE("serial recv tail failed, char = %d", recv_byte);
    }
    g_serial.recv_cache_index = 0;
  } else {
    g_serial.recv_cache[g_serial.recv_cache_index] = recv_byte;
    g_serial.recv_cache_index++;
  }
}

int serial_send(unsigned char *buf, int n) {
  if (1) {
  return _push_into_list(buf, n, &g_serial.send_list, &g_serial.send_list_mutex);
  } else {
    g_request_id = buf[5];
    return 0;
  }
}

static int _uart_set_speed() {
  struct termios options;
  int status;
  tcgetattr(g_serial.uart_fd, &options);
  tcflush(g_serial.uart_fd, TCIOFLUSH);
  cfsetispeed(&options, B9600);
  status = tcsetattr(g_serial.uart_fd, TCSANOW, &options);
  if (0 != status) {
    perror("error: tcsetattr failed!");
    return -1;
  }
  tcflush(g_serial.uart_fd, TCIOFLUSH);
  return 0;
}

static int _uart_set_parity() {
  struct termios options;
  if (tcgetattr(g_serial.uart_fd, &options) != 0) {
    perror("error _uart_set_parity:tcgetattr.");
    return -1;
  }
  cfmakeraw(&options); /* 配置为原始模式 */
  options.c_cflag &= ~CSIZE; //屏蔽数据位
  options.c_cflag |= CS8; //数据位为8位
  options.c_cflag &= ~CSTOPB; //1位停止位
  options.c_cflag &= ~PARENB; //无校验
  //options.c_cflag &= ~PARODD; //偶校验
  //options.c_iflag |= INPCK; //使能奇偶校验

  /*设置等待时间和最小接收字符*/
  options.c_cc[VTIME] = 0;
  options.c_cc[VMIN] = 1;
  if (tcsetattr(g_serial.uart_fd, TCSANOW, &options) != 0) {
    perror("error _uart_set_parity:tcsetattr");
    return -1;
  }
  tcflush(g_serial.uart_fd, TCIOFLUSH);
  return 0;
}

static int _uart_send(unsigned char *buf, int n) {
  int rc = 0;
  int i = 0;
  rc = write(g_serial.uart_fd, buf, n);
  LOGT("serial send %s >>>>>>>>>>>>>>>>>>>", rc < 0 ? "failed" : "succeed");
  for (i = 0; i < n; i++) {
    printf("%02x ", buf[i]);
  }
  printf("\n");
  return rc;
}

static void *_sending_process(void *arg) {
  unsigned char data[FRAME_LEN_MAX];
  int n;
  while (1) {
    while (_pull_from_list(data, &n, &g_serial.send_list,
                           &g_serial.send_list_mutex) != -1) {
      if (_uart_send(data, n) < 0) {
        LOGE("uart send error!");
      }
    }
    usleep(100 * 1000);  //100ms
  }
  return 0;
}

static void *_recving_process() {
  char recv_byte;
  fd_set rfds;
  struct timeval tv;
  int retval;
  while (1) {
    FD_ZERO(&rfds);
    FD_SET(g_serial.uart_fd, &rfds);
    tv.tv_sec = 0;
    tv.tv_usec = 80 * 1000;
    retval = select(g_serial.uart_fd + 1, &rfds, NULL, NULL, &tv);
    if (retval < 0) {
      perror("select()");
    } else if (retval > 0) {
      if (FD_ISSET(g_serial.uart_fd, &rfds)) {
        read(g_serial.uart_fd, &recv_byte, 1);
        _deal_with_recv_byte(recv_byte);
      }
    }
  }
  return NULL;
}

static int _init_uart() {
  const char device[] = "/dev/ttyS0";
  list_init(&g_serial.send_list);
  list_init(&g_serial.recv_list);
  pthread_mutex_init(&g_serial.send_list_mutex, NULL);
  pthread_mutex_init(&g_serial.recv_list_mutex, NULL);
  g_serial.uart_fd = open(device, O_RDWR | O_NOCTTY | O_NDELAY);
  if (-1 == g_serial.uart_fd) {
    LOGE("open %s failed", device);
    return -1;
  }
  if (_uart_set_speed() < 0) {
    LOGE("uart set speed error !");
    return -1;
  }
  if (_uart_set_parity() < 0) {
    LOGE("uart set parity error !");
    return -1;
  }
  return 0;
}

static int _init_thread() {
  pthread_t recving_pid;
  pthread_t sending_pid;
  int ret = pthread_create(&recving_pid, NULL, &_recving_process, NULL);
  if (ret != 0) {
    LOGE("_recving_process pthread_create fail!");
    return ret;
  }
  LOGE("create recving thread success");
  usleep(500 * 1000);
  ret = pthread_create(&sending_pid, NULL, &_sending_process, NULL);
  if (ret != 0) {
    LOGE("_sending_process pthread_create fail!");
    return ret;
  }
  LOGE("create sending thread success");
  return 0;
}

int serial_init() {
  if (0 != _init_uart()) {
    return -1;
  }
  LOGT("uart inited");
  if (0 != _init_thread()) {
    return -1;
  }
  LOGT("serial thread inited");
  return 0;
}

void serial_release() {
  close(g_serial.uart_fd);
}
