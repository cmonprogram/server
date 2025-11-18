#include "main.h"
#include "arg.h"
#include "stdio.h"

int main(int argc, char** argv){
    server_settings settings;
    if(parce_settings(&settings, argc, argv)){
        printf("Settings ok\n");
    }
}