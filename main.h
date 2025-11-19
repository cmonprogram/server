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
} server_params;

#ifndef DISABLE_PRINT
#define PRINT(...) printf(__VA_ARGS__)
#else
#define PRINT(...) {};
#endif

#define EXECUTE_STAGE(stage_name, stage_func, ...)                             \
  if (stage_func(__VA_ARGS__)) {                                               \
    PRINT("[ok] %s\n", stage_name);                                            \
  } else {                                                                     \
    PRINT("[fail] %s\n", stage_name);                                          \
    PRINT("[exit]\n");                                                         \
    return 1;                                                                  \
  }

#endif