#include "arg.h"
#include <stdio.h>
#include <string.h>

void print_usage() {
  PRINT("%s:  usage:  %s [-t | -u] port \n", PROGRAMM_NAME, PROGRAMM_NAME);
}

void print_description() {
  PRINT("    Strarts server with endpoint /time, /shutdown etc at spesified "
        "port \n");
}

void print_help() {
  print_usage();
  print_description();
  PRINT("    Options: \n"
        "       -t tcp\n"
        "       -u udp\n");
}

int parce_protocol(server_settings *settings, char *protocol) {
  if (strcmp(protocol, "-t") == 0) {
    settings->protocol = TCP;
    return 1;
  } else if (strcmp(protocol, "-u") == 0) {
    settings->protocol = UDP;
    return 1;
  } else {
    return 0;
  }
}

int parce_port(server_settings *settings, char *port) {
  int port_no = 0;
  int result = sscanf(port, "%d", &port_no);
  if (result && result != EOF) {
    if (port_no > 0) {
      settings->port_no = port_no;
      return 1;
    } else {
      return 0;
    }
  } else {
    return 0;
  }
}

int parce_settings(server_settings *settings, int argc, char **argv) {
  if (argc == 2) {
    if (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0) {
      print_help();
      return 0;
    } else {
      print_usage();
      return 0;
    }
  } else if (argc == 3) {
    if (!parce_protocol(settings, argv[1]) || !parce_port(settings, argv[2])) {
      print_usage();
      return 0;
    } else {
      return 1;
    }
  } else {
    print_usage();
    return 0;
  }

  // not use
  return 0;
}