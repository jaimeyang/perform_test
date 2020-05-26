//
// Created by jaime on 2020/5/22.
//环形队列，避免加锁操作
//这样就会留出了多一个空位用来判断
//

#ifndef PERFROMTEST_CIRQUEUE_H
#define PERFROMTEST_CIRQUEUE_H

#include <iostream>

template<typename T>
class CirQueue {
public:
    explicit CirQueue(int size):
    m_size(size)
    {
        m_data = new T[m_size];
    }
    ~CirQueue(){

    }
    bool enqueue(T data){
        if (this->full()){
            printf("is full\n");
            return false;
        }
        printf("no full\n");
        this->m_data[ this->m_write_pos ] = data;
        this->m_write_pos = ( this->m_write_pos + 1 ) % this->m_size;
        return true;
    }
    bool dequeue(T& data){
        if (this->empty()){
            return false;
        }
        data = this->m_data[ this->m_read_pos ];
        this->m_read_pos = ( this->m_read_pos + 1 ) % this->m_size;
        return true;
    }
    bool empty(){
        if (( this->m_read_pos % this->m_size ) == ( this->m_write_pos % this->m_size )){
            return true;
        }
        return false;
    }
    bool full(){
        if (( ( this->m_write_pos + 1 ) % this->m_size ) == ( this->m_read_pos % this->m_size )){
            return true;
        }
        return false;
    }

private:
    int m_size = 0;
    //强制从内存重新取值
    volatile int m_write_pos = 0;
    volatile int m_read_pos = 0;
    T* m_data;
};


#endif //PERFROMTEST_CIRQUEUE_H
