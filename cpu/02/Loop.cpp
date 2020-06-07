//
// Created by jaime on 2020/5/21.
//

#include <zconf.h>
#include <cstring>
#include "Loop.h"
#include "LibEvent.h"
#include <math.h>

void Loop::work_loop() {
    this->m_event->init_event();
    bool isLoopRunning = false;
    const char* data = new char[1024];
    while ( true ){
        int fd = 0;
        if ( this->get_fd(fd) ){
            this->m_event->read_event([this,data](int fd) mutable {
                int n = 0;
                int len = 0;

                char* p = const_cast<char *>(data);
                while ( ( n = read(fd, (void *) p, 1024) ) < 0 ){
                    printf("errnro is %s\n",strerror(errno));
                    if ( (errno == EAGAIN) || (errno == EWOULDBLOCK) ){
                        continue;
                    }
                    //close socket
                    if ( errno == ECONNRESET ){
                        this->m_event->del_event(fd);
                        return;
                    }
                }
                //close socket
                if( n == 0 ){
                    this->m_event->del_event(fd);
                    return;
                }
                if ( n > 0 ){
                    len += n;
                }
                //todo test
                float ret = 10000000;
                for (int i = 0; i < 1000000000; ++i) {
                    ret += sqrt(ret);
                }
                int w_ret = 0;
                while ( (w_ret = write(fd,p,len)) < len){
                    printf("errnro is %s\n",strerror(errno));
                    if ( w_ret < 0 ){
                        if ( (errno == EAGAIN) || (errno == EWOULDBLOCK) ){
                            continue;
                        }
                        return;
                    }
                    p = p + w_ret;
                    len = len - w_ret;
                }
            },fd);
        }
        this->m_event->loop_exit();
        this->m_event->loop_event(EVLOOP_NO_EXIT_ON_EMPTY);
    };
}

void Loop::read_client(int fd) {

}

void Loop::write_client(int fd, const char *content) {

}
