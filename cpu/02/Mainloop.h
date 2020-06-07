//
// Created by jaime on 2020/5/21.
//

#ifndef PERFROMTEST_MAINLOOP_H
#define PERFROMTEST_MAINLOOP_H


#include "Loop.h"
#include "Utils.h"

class Mainloop {
public:
    Mainloop(){
        this->m_cpu_core = get_cpu_core();
        this->m_workers = new Loop*[this->m_cpu_core];
    }
public:
    void main_loop();
    void push_worker(Loop* l);
private:
    int get_cur_cpu_num(){
        return m_now_worker % m_cpu_core;
    }
private:
    Loop** m_workers = nullptr;
    int m_now_worker = 0;
    int m_cpu_core = 0;
};


#endif //PERFROMTEST_MAINLOOP_H
