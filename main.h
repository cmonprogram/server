#ifndef MAIN_H
#define MAIN_H

#define PROGRAMM_NAME "server"

typedef enum { TCP, UDP } PROTOCOL;

typedef struct {
  PROTOCOL protocol;
  int port_no;
  int status;
} server_settings;

#ifndef DISABLE_PRINT
    #define PRINT(...) printf(__VA_ARGS__)
#else
    #define PRINT(...) {};
#endif

#endif