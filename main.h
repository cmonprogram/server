#ifndef MAIN_H
#define MAIN_H

#include <netinet/in.h>
#include <sys/socket.h>


#define PROGRAMM_NAME "server"

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
  long long server_test_start_milliseconds;
  long long server_test_end_milliseconds;

} server_params;

typedef struct {
  char in_buffer[1000];
  char out_buffer[1000];
  struct sockaddr_in client_addr;
  socklen_t addr_len;
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
#define PRINT_GREEN(status, data)PRINT("[%s%s%s] %s\n", KGRN, status, KNRM, data)
#define PRINT_RED(status, data) PRINT("[%s%s%s] %s\n", KRED, status, KNRM, data)
#define PRINT_ERROR(data)  PRINT_RED("error", data)


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