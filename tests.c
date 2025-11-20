#include "arg.h"
#include "main.h"
#include "server.h"
#include <assert.h>
#include <stdio.h>

#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

int udp_load_test_run_server() {
  server_settings settings;
  settings.port_no = 8080;
  settings.protocol = UDP;
  return server_run(&settings);
}

int udp_load_test_run_client() {
  for (int i = 0; i < 10; ++i) {
    printf("%d\n", i);
  }
  return 1;
}

  // lod test, output number of processed packages (with return) per 30s (run sequentially / parallel)
int start_udp_load_test() {
  pid_t pid;
  switch (pid = fork()) {
  case -1:
    perror("fork");
    return 0;
  case 0:
    udp_load_test_run_server();
    exit(0);
  default:
    udp_load_test_run_client();
    int status;
    wait(&status);
  }
  return 1;
}

int run_settings_test() {
  server_settings settings;
  assert(parce_settings(&settings, 1, (char *[]){"a"}) == 0);
  assert(parce_settings(&settings, 2, (char *[]){"a", "b"}) == 0);
  assert(parce_settings(&settings, 3, (char *[]){"a", "b", "c"}) == 0);
  return 1;
}

int main() {
  RUN_TEST("settings_test", run_settings_test);
  RUN_TEST("udp_load_test", start_udp_load_test);
  printf("Test passed!\n");
}