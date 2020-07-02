#include <stdio.h>
#include <stdbool.h>
#include <zconf.h>
#include <string.h>
#include <stdlib.h>

void leak(){
    char* c = (char*)calloc(1000,sizeof(char));
    for (int i = 0; i < 1000; ++i) {
        *c = 'a';
    }
    return;
}


int main() {
    while (true){
        leak();
        sleep(1);
    }
    return 0;
}
