//
// Created by jaime on 2020/5/21.
//

#ifndef PERFROMTEST_LOOP_H
#define PERFROMTEST_LOOP_H


#include "CirQueue.h"
#include "LibEvent.h"
#include <map>

class Loop {
public:
    //todo the size can config
    Loop():
    m_queue(new CirQueue<int>(1024)),m_event(new LibEvent)
    {

    }
    ~Loop(){

    }
public:
    void work_loop();
    bool push_fd(int fd){
        return m_queue->enqueue(fd);
    }
    bool get_fd(int& fd){
        return m_queue->dequeue(fd);
    }

private:
    void read_client(int fd);
    void write_client(int fd,const char* content);
private:
    CirQueue<int>* m_queue;
    LibEvent* m_event;
};


#endif //PERFROMTEST_LOOP_H
