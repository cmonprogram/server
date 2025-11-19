#include "main.h"
#include "arg.h"
#include <assert.h>
#include <stdio.h>

int main(){
        server_settings settings;
        assert(parce_settings(&settings, 1, (char *[]){"a"} ) == 0);
        assert(parce_settings(&settings, 2, (char *[]){"a", "b"} ) == 0);
        assert(parce_settings(&settings, 3, (char *[]){"a", "b","c"} ) == 0);
        printf("Test passed!\n");
}