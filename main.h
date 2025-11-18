#ifndef MAIN_H
#define MAIN_H

#define PROGRAMM_NAME "server"

typedef enum { TCP, UDP } PROTOCOL;

typedef struct {
  PROTOCOL protocol;
  int port_no;
  int status;
} server_settings;

#endif