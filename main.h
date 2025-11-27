#ifndef MAIN_H
#define MAIN_H

#include <netinet/in.h>
#include <sys/epoll.h>
#include <sys/socket.h>

#define PROGRAMM_NAME "server"
#define MAX_EVENTS 10
#define MAX_ARGS 10
#define MSG_BUFFER_SIZE 1000

typedef enum { TCP, UDP } PROTOCOL;
typedef enum { RESULT_FAIL, RESULT_SUCESS, RESULT_EXIT } RESULT;

typedef struct {
  PROTOCOL protocol;
  int port_no;
} server_settings;

typedef struct {
  int sock_fd;
  struct sockaddr_in server_addr;
  int server_in_test;
  int server_test_packages_number;
  int server_test_packages_number_end;
  long long server_test_start_milliseconds;
  long long server_test_end_milliseconds;
  // epoll support
  int epollfd;
  struct epoll_event events[MAX_EVENTS];
  struct epoll_event epoll_ev_listen;
} server_params;

typedef struct {
  char in_buffer[MSG_BUFFER_SIZE];
  int in_buffer_len;
  char out_buffer[MSG_BUFFER_SIZE];
  int out_buffer_len;
  char *args[MAX_ARGS];
  int client_fd;
  struct sockaddr_in client_addr;
  socklen_t addr_len;
  // epoll support
  int epoll_ev_fd;
} request_instance;

#define KGRN "\x1B[32m"
#define KRED "\x1B[31m"
#define KNRM "\x1B[0m"

/*
#ifndef DISABLE_PRINT
#define PRINT(...) printf(__VA_ARGS__)
#else
#define PRINT(...) {};
#endif
*/

#define PRINT(...) printf(__VA_ARGS__)
#define PRINT_NORMAL(status, data) PRINT("[%s] %s\n", status, data)
#define PRINT_GREEN(status, data)                                              \
  PRINT("[%s%s%s] %s\n", KGRN, status, KNRM, data)
#define PRINT_RED(status, data) PRINT("[%s%s%s] %s\n", KRED, status, KNRM, data)
#define PRINT_ERROR(data) PRINT_RED("err", data)

#define EXECUTE_STAGE(stage_name, stage_func, ...)                             \
  {                                                                            \
    RESULT res = stage_func(__VA_ARGS__);                                      \
    switch (res) {                                                             \
    case RESULT_SUCESS:                                                        \
      PRINT_GREEN("ok", stage_name);                                           \
      break;                                                                   \
    case RESULT_FAIL:                                                          \
      PRINT_RED("fail", stage_name);                                           \
      return RESULT_FAIL;                                                      \
      break;                                                                   \
    case RESULT_EXIT:                                                          \
      PRINT("[exit]\n");                                                       \
      return RESULT_SUCESS;                                                    \
      break;                                                                   \
    default:                                                                   \
      PRINT_RED("error", stage_name);                                          \
      return RESULT_FAIL;                                                      \
    }                                                                          \
  }

#define RUN_TEST(test_name, test_func)                                         \
  {                                                                            \
    if (test_func()) {                                                         \
      PRINT_GREEN("passed", test_name);                                        \
    } else {                                                                   \
      PRINT_RED("failed", test_name);                                          \
    }                                                                          \
  }

#endif