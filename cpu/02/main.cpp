#include <iostream>
#include <thread>
#include <zconf.h>
#include "Loop.h"
#include "Mainloop.h"
#include "Utils.h"

using namespace std;

void start_new_thread(Mainloop* m){
    int cpu_core = get_cpu_core();
    for (int i = 0; i < cpu_core; ++i) {
        Loop* l = new Loop;
        thread t1(&Loop::work_loop,l);

        m->push_worker(l);
        t1.detach();
    }
}



int main() {
//    Mainloop* m = new Mainloop;
//
//    start_new_thread(m);

//    m->main_loop();



    while (true){
        sleep(10);
    }

    return 0;
}
