#include <stdio.h>
#include <stdbool.h>
#include <zconf.h>


void leak(){
    char* c = (char*)malloc(1);
    return;
}


int main() {
    while (true){
        leak();
        sleep(1);
    }
    return 0;
}
