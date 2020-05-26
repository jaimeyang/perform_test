//
// Created by jaime on 2020/5/21.
//

#include "Mainloop.h"
#include "LibEvent.h"
#include "Utils.h"

void Mainloop::main_loop() {
    LibEvent* l = new LibEvent;
    l->server_listen("0.0.0.0",4000);
    l->accept_event([this](int fd){
        int index = this->get_cur_cpu_num();
        m_now_worker = ( m_now_worker + 1 ) % m_cpu_core;
        Loop* l = this->m_workers[index];
        bool is_scuess = l->push_fd(fd);
        if ( is_scuess ){
            return ;
        }
    });
    l->loop_event();
}

void Mainloop::push_worker(Loop *l) {
    int index = get_cur_cpu_num();
    m_workers[index] = l;
    m_now_worker = ( m_now_worker + 1 ) % m_cpu_core;
}
