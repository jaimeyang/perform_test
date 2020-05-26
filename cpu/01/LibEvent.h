//
// Created by jaime on 2020/5/21.
//

#ifndef PERFROMTEST_LIBEVENT_H
#define PERFROMTEST_LIBEVENT_H

#include <event2/event.h>
#include <functional>
#include <map>

using namespace std;

using accept_func = function<void(int)>;
using read_func = function<void(int)>;

class LibEvent {
public:
    LibEvent(){

    }
    ~LibEvent(){

    }
    bool server_listen(const char* ip,int port);
    void init_event();
    void accept_event(accept_func fun);
    void accept_cb(int afd);

    void read_event(read_func fun,int fd);
    void read_cb(int afd);

    void write_event();
    void loop_event(int flags = 0);

    void del_event(int fd);

    void loop_exit();
    void loop_continue();
private:

private:

    event_base* m_base = nullptr;
    int m_fd = 0;
    accept_func m_assept_func;
    read_func m_read_func;
    map<int,event*> m_all_events;
};


#endif //PERFROMTEST_LIBEVENT_H
