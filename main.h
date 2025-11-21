#ifndef MAIN_H
#define MAIN_H

#include <netinet/in.h>

#define PROGRAMM_NAME "server"

typedef enum { TCP, UDP } PROTOCOL;

typedef struct {
  PROTOCOL protocol;
  int port_no;
} server_settings;

typedef struct {
  int sock_fd;
  struct sockaddr_in server_addr, client_addr;
  socklen_t addr_len;
  char in_buffer[1000];
  char out_buffer[1000];
} server_params;

#define RESULT_SUCESS 1
#define RESULT_FAIL 0

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

#define EXECUTE_STAGE(stage_name, stage_func, ...)                             \
  if (stage_func(__VA_ARGS__)) {                                               \
    PRINT("[%sok%s] %s\n", KGRN, KNRM, stage_name);                            \
  } else {                                                                     \
    PRINT("[%sfail%s] %s\n", KRED, KNRM, stage_name);                          \
    PRINT("[exit]\n");                                                         \
    return RESULT_FAIL;                                                                \
  }

#define RUN_TEST(test_name, test_func)                                         \
  {                                                                            \
    if (test_func()) {                                                         \
      printf("[%spassed%s] %s\n", KGRN, KNRM, test_name);                      \
    } else {                                                                   \
      printf("[%sfailed%s] %s\n", KRED, KNRM, test_name);                      \
    }                                                                          \
  }

#endif