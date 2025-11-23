#ifndef ARG_H
#define ARG_H
#include "main.h"

void print_usage();
void print_description();
void print_help();
int parce_protocol(server_settings *settings, char *protocol);
int parce_port(server_settings *settings, char *port);
int parce_settings(server_settings *settings, int argc, char **argv);
int parce_args(char *args, int length, char **args_array, int args_array_lengh);
#endif