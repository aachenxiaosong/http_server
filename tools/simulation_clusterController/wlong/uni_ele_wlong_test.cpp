#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <vector>
#include <evhttp.h>
#include <event.h>
#include <string.h>
#include <pthread.h>
#include "event2/http.h"
#include "event2/event.h"
#include "event2/buffer.h"
#include "event2/bufferevent.h"
#include "event2/bufferevent_compat.h"
#include "event2/http_struct.h"
#include "event2/http_compat.h"
#include "event2/util.h"
#include "event2/listener.h"


#define BUF_MAX              (1024 * 16)
#define HTTP_SERVER_TAG      "http_server"
#define SERVER_MAX_THREAD_NUM 100
#define SERVER_LISTEN_PORT    5000

typedef struct httpd_info {
  struct event_base *base;
  struct evhttp *httpd;
} httpd_info;
pthread_t g_threads[SERVER_MAX_THREAD_NUM];
httpd_info g_info_arr[SERVER_MAX_THREAD_NUM];
int server_port = SERVER_LISTEN_PORT;

//解析http url中的path
static int find_http_path(struct evhttp_request *req, char *result) {
  if(req == NULL) {
    printf("wrong params\n");
    return -1;
  }
  struct evhttp_uri *decoded = NULL;
  char *query = NULL;
  char *query_result = NULL;
  const char *path;
  const char *uri = evhttp_request_get_uri(req);//获取请求uri
  if(uri == NULL) {
    printf("get uri failed\n");
    return -1;
  }

  //解码uri
  decoded = evhttp_uri_parse(uri);
  if (!decoded) {
    printf("parse url failed\n");
    evhttp_send_error(req, HTTP_BADREQUEST, 0);
    return -1;
  }

  //获取uri中的path部分
  path = evhttp_uri_get_path(decoded);
  if (path == NULL) {
    path = "/";
  }
  printf("get path: %s\n", path);
  strcpy(result, path);
  evhttp_uri_free(decoded);
  return 0;
}


//解析post请求数据
int get_post_request(char *request, struct evhttp_request *req) {
  size_t post_size = 0;
  size_t copy_len;
  post_size = evbuffer_get_length(req->input_buffer);//获取数据长度
  if (post_size <= 0) {
    printf("post request is empty!\n");
    return -1;
  }
  copy_len = post_size > BUF_MAX ? BUF_MAX : post_size;
  memcpy(request, evbuffer_pullup(req->input_buffer, -1), copy_len);
  request[copy_len] = '\0';
  printf("get post request: %s\n", request);
  return 0;
}

//解析http头，主要用于get请求时解析uri和请求参数
int find_http_header(struct evhttp_request *req, const char *query_char, char *result) {
  struct evkeyvalq params = {0};
  struct evhttp_uri *decoded = NULL;
  char *query = NULL;
  char *query_result = NULL;
  const char *uri;
  if(req == NULL || query_char == NULL || result == NULL) {
    printf("wrong params\n");
    return -1;
  }
  uri = evhttp_request_get_uri(req);//获取请求uri
  if(uri == NULL) {
    printf("get uri failed\n");
    return -1;
  }
  printf("get request uri: %s\n", uri);
  //解码uri
  decoded = evhttp_uri_parse(uri);
  if (!decoded) {
    printf("parse url failed\n");
    evhttp_send_error(req, HTTP_BADREQUEST, 0);
    return -1;
  }
  //获取uri中的参数部分
  query = (char*)evhttp_uri_get_query(decoded);
  if(query == NULL) {
    printf("get query failed\n");
    evhttp_uri_free(decoded);
    return -1;
  }

  //查询指定参数的值
  evhttp_parse_query_str(query, &params);
  query_result = (char*)evhttp_find_header(&params, query_char);
  evhttp_uri_free(decoded);
  strcpy(result, query_result);
  return 0;
}

//处理get请求
void http_handler_testget_msg(struct evhttp_request *req,void *arg) {
  int ret;
  char sign[64] = {0};
  char data[64] = {0};
  struct evbuffer *retbuff = NULL;
  if(req == NULL) {
    printf("req is null\n");
    return;
  }
  ret = find_http_header(req, "sign", sign);//获取get请求uri中的sign参数
  if(ret != 0) {
    printf("get param sign failed\n");
  }
  ret = find_http_header(req, "data", data);//获取get请求uri中的data参数
  if(ret != 0) {
    printf("get param data failed\n");
  }
  printf("get request param sign:%s data:%s\n", sign, data);
  //回响应
  retbuff = evbuffer_new();
  if(retbuff == NULL) {
    printf("alloc retbuff failed\n");
    return;
  }
  evbuffer_add_printf(retbuff,"Receive get request,Thanks for the request!");
  evhttp_send_reply(req,HTTP_OK,"Client",retbuff);
  evbuffer_free(retbuff);
}

int process_post_request(char *path, char *request, char *response) {
  static int floor_no = 1;
  static int left_dir = 0;

  if (strcmp(path, "/eiserver/client/systemDockingDeviceInfo/getElevatorStatus") == 0) {
  	floor_no++;
	if(floor_no > 5)
		floor_no = 1;

	left_dir++;
	if(left_dir > 2)
		left_dir = 0;
    sprintf(response, "{\"code\": 0, \"message\":\"OK\", \"data\": {\"status\":\"00%02d%02d020000\"}}", floor_no, left_dir);
  } else {
    sprintf(response, "%s", "{\"code\": 0, \"message\":\"OK\", \"data\":\"no data\"}");
  }

  return 0;
}

//处理post请求
void http_handler_post_msg(struct evhttp_request *req,void *arg) {
  int ret;
  char path[128] = {0};
  char request[BUF_MAX] = {0};
  char response[BUF_MAX] = {0};
  struct evbuffer *retbuff = NULL;
  if(req == NULL) {
    printf("req is null\n");
    return;
  }
  if (req->type != EVHTTP_REQ_POST) {
    printf("not a post request\n");
    return;
  }
  printf("post request handled by thread %ld\n", pthread_self());
  find_http_path(req, path);
  ret = get_post_request(request, req);//获取请求数据，一般是json格式的数据
  if(ret != 0) {
    printf("get post request failed\n");
    return;
  }
  //处理请求，可以使用json库解析需要的数据
  process_post_request(path, request, response);
  //回响应
  retbuff = evbuffer_new();
  if(retbuff == NULL) {
    printf("alloc retbuff failed\n");
    return;
  }
  evhttp_add_header(req->output_headers, "Content-Type", "application/json; charset=UTF-8");
  evbuffer_add_printf(retbuff, "%s", response);
  evhttp_send_reply(req, HTTP_OK, "Client", retbuff);
  evbuffer_free(retbuff);
}

/*static void* _httpserver_routine(void *param) {
  //start http server here
  struct evhttp *http_server = NULL;
  short http_port = 8080;
  const char *http_addr = "0.0.0.0";
  //初始化
  event_init();
  //启动http服务端
  http_server = evhttp_start(http_addr,http_port);
  if(http_server == NULL) {
    LOGE(HTTP_SERVER_TAG, "http server starts failed");
    return NULL;
  }

  //设置请求超时时间(s)
  evhttp_set_timeout(http_server, 10);
  //设置事件处理函数，evhttp_set_cb针对每一个事件(请求)注册一个处理函数，
  //区别于 evhttp_set_gencb 函数，是对所有请求设置一个统一的处理函数
  //evhttp_set_cb(http_server,"/liftCtrl/v1/callLift", http_handler_post_msg, NULL);
  evhttp_set_gencb(http_server, http_handler_post_msg, NULL);

  //循环监听
  event_dispatch();
  //实际上不会释放，代码不会运行到这一步
  evhttp_free(http_server);
  
  return NULL;
}*/

void *dispatch(void *args){
  struct httpd_info *info = (struct httpd_info *)args;
  printf("http_server thread %ld start\n", pthread_self());
  event_base_dispatch(info->base);
  printf("http_server thread %ld done\n", pthread_self());
  event_base_free(info->base);
  evhttp_free(info->httpd);
  return NULL;
}

int bind_socket() {
  int ret, server_socket, opt=1;
  server_socket = socket(AF_INET,SOCK_STREAM|SOCK_NONBLOCK,0);//NOTE 多线程evhttp必须非阻塞
  if (server_socket<0) {
    printf("get socket failed\n");
    return -1;
  }
  setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
  struct sockaddr_in addr;
  memset(&addr,0,sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = INADDR_ANY;
  addr.sin_port = htons(server_port);
  ret = bind(server_socket, (struct sockaddr*)&addr, sizeof(struct sockaddr));
  if(ret<0) {
    printf("bind failed\n");
    close(server_socket);
    return -1;
  }
  printf("bind succ, port:%d\n", server_port);
  listen(server_socket, 1024);
  
  return server_socket;
}

int http_server_start() {
  httpd_info *pinfo;
  int i;
  int ret;
  int server_socket;
  server_socket = bind_socket();
  if (-1 == server_socket) {
    printf("bind socket failed\n");
    return -1;
  }
  int thread_num = 1;
  if (thread_num > SERVER_MAX_THREAD_NUM) {
    thread_num = SERVER_MAX_THREAD_NUM;
  }
  for(i = 0; i< thread_num; i++) {
    pinfo = &g_info_arr[i];
    pinfo->base = event_base_new();
    if (pinfo->base == NULL) {
      printf("new event_base failed\n");
      return -1;
    }
    pinfo->httpd = evhttp_new(pinfo->base);
    if (pinfo->httpd == NULL) {
      printf("new evhttp failed\n");
      return -1;
    }
    ret = evhttp_accept_socket(pinfo->httpd, server_socket);
    if (ret != 0) {
      printf("evhttp_accept_socket failed\n");
      return -1;
    }
	
    evhttp_set_timeout(pinfo->httpd, 10);
    evhttp_set_gencb(pinfo->httpd, http_handler_post_msg, NULL);
    pthread_create(&g_threads[i], NULL, dispatch, pinfo);
    pthread_detach(g_threads[i]);
  }
  return 0;
}


int http_server_clear_handler() {
  //g_http_handler_lock.writeLock();
  //g_http_handlers.clear();
  //g_http_handler_lock.writeUnlock();
  return 0;
}


int main(int argc ,char *argv[])
{
  if(argc < 2) {
    fprintf(stderr,"Usage:%s port\a\n", argv[0]);
    return 0;
  }

  server_port = atoi(argv[1]);
  if(server_port <= 0) {
    server_port = SERVER_LISTEN_PORT;
  }
  http_server_start();
  while(1) {
    sleep(10);
  }
}
