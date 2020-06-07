//
// Created by jaime on 2020/5/21.
//

#include "LibEvent.h"
#include <memory>
#include <cstring>
#include <arpa/inet.h>
#include <event.h>
#include <unistd.h>

void on_accept(int fd,short event,void* arg){
    LibEvent* l = (LibEvent*)arg;

    struct sockaddr_in in;
    socklen_t len = sizeof(in);
    int afd = accept(fd,(struct sockaddr*)&in,&len);
    if(afd == -1){
        printf("error is %s\n",strerror(errno));
        return;
    }
    l->accept_cb(afd);
}

void on_read(int fd,short event,void* arg){
    LibEvent* l = (LibEvent*)arg;
    l->read_cb(fd);
}

bool LibEvent::server_listen(const char *ip, int port) {
    sockaddr_in in;
    memset(&in,0,sizeof(sockaddr_in));
    in.sin_family = AF_INET;
    in.sin_addr.s_addr = inet_addr(ip);
    in.sin_port = htons(port);
    int fd = socket(AF_INET,SOCK_STREAM,0);
    bind(fd,(struct sockaddr*)&in,sizeof(in));
    int ret_l = listen(fd,2);
    if (ret_l == -1){
        printf("error is %s\n",strerror(errno));
        return false;
    }

    this->init_event();
    this->m_fd = fd;
}


void LibEvent::init_event() {
    this->m_base = event_base_new();
}


void LibEvent::loop_event(int flag) {
    event_base_loop(this->m_base,flag);
}

void LibEvent::accept_event(accept_func fun) {
    //todo 这个要水平触发
    event* e = event_new(this->m_base, this->m_fd, EV_READ | EV_PERSIST, on_accept, (void*)this);
    event_add(e, nullptr);
    this->m_assept_func = fun;
}

void LibEvent::accept_cb(int afd) {
//    printf("afd is %d\n",afd);
    this->m_assept_func(afd);
}

void LibEvent::read_event(read_func fun,int fd) {
    //todo 这个要水平触发
    event* e = event_new(this->m_base,fd, EV_READ | EV_PERSIST, on_read, (void*)this);
    event_add(e, nullptr);
    this->m_all_events[fd] = e;
    this->m_read_func = fun;
}

void LibEvent::read_cb(int afd) {
//    printf("read_cb afd is %d\n",afd);
    this->m_read_func(afd);
}

void LibEvent::write_event() {

}

void LibEvent::loop_exit() {
    struct timeval sec;
    sec.tv_sec = 0;
    sec.tv_usec = 1;
    event_base_loopexit(this->m_base,&sec);
}

void LibEvent::loop_continue() {
    event_base_loopcontinue(this->m_base);
}

void LibEvent::del_event(int fd) {
    event* e = this->m_all_events[fd];
    event_del(e);
    close(fd);
}




