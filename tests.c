#include "arg.h"
#include "main.h"
#include "socket.h"
#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <arpa/inet.h>

int udp_load_test_run_server() {
  server_settings settings;
  settings.port_no = 8080;
  settings.protocol = UDP;
  return server_run(&settings);
}

/*
void *PrintI(void *args) {
  printf("th %d\n", *(int *)args);
  pthread_exit(NULL);
}

int udp_load_test_run_client() {
  pthread_t threads[10];
  for (int i = 0; i < 10; ++i) {
    pthread_create(&threads[i], NULL, PrintI, (void *)&i);
  }
  return 1;
}
*/

// https://www.geeksforgeeks.org/computer-networks/udp-client-server-using-connect-c-implementation/
int udp_load_test_run_client() {
  char in_buffer[100];
  char out_buffer[100];
  char *message = "Hello Server";
  int sockfd, n;
  struct sockaddr_in servaddr;

  // clear servaddr
  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
  servaddr.sin_port = htons(8080);
  servaddr.sin_family = AF_INET;

  // create datagram socket
  sockfd = socket(AF_INET, SOCK_DGRAM, 0);

  // connect to server
  if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
    printf("\n Error : Connect Failed \n");
    exit(0);
  }

  for (int i = 0; i < 1000000; ++i) {
    sprintf(out_buffer, "Hello web server %d", i);
    sendto(sockfd, out_buffer, sizeof(out_buffer), 0, (struct sockaddr *)NULL,
           sizeof(servaddr));
    // sleep(1);
  }

  /*
  for (int i = 0; i < 3; ++i) {
  printf("exit send %d\n", i);
  sleep(1);
  }
  */

  sprintf(out_buffer, "exit");
  sendto(sockfd, out_buffer, sizeof(out_buffer), 0, (struct sockaddr *)NULL,
         sizeof(servaddr));
  

  // waiting for response
  // recvfrom(sockfd, in_buffer, sizeof(in_buffer), 0, (struct sockaddr*)NULL, NULL);
  // puts(in_buffer);

  // close the descriptor
  close(sockfd);

  return 0;
}

// lod test, output number of processed packages (with return) per 30s (run
// sequentially / parallel)
// package nuber and test results gets from server-side
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