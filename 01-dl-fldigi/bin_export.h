#ifndef BIN_EXPORT_H
#define BIN_EXPORT_H

#include <stddef.h>
#include <stdint.h>
#include <pthread.h>
#include <string>

template <typename T, size_t S>
class RingBuffer {
public:
    
    typedef T element_type;
    
    RingBuffer() : head_(0), tail_(0) {}
    
    ~RingBuffer() {}
    
    void push(T element) {
        size_t next_head = (head_ + 1) % S;
        if(next_head == tail_) { return; }
        data_[head_] = element;
        head_ = next_head;
    }
    
    T pop() {
        if(head_ == tail_) { return T(); }
        T element = data_[tail_];
        tail_ = (tail_ + 1) % S;
        return element;
    }
    
    size_t size() {
        return (S + head_ - tail_) % S;
    }
    
private:
    
    size_t  head_;
    size_t  tail_;
    T       data_[S];
};

class BinExporter {
public:
    
    BinExporter();
    
    ~BinExporter();
    
    void logByte(uint8_t byte);
    
    void logByteSync(uint8_t byte);
    
    // Must be public because pthread_create can't accept member fun.
    RingBuffer<uint8_t, 512>    ipcBuffer;
    pthread_cond_t              ipcCond;
    pthread_mutex_t             ipcMutex;
    bool                        quit;
    
private:
    
    std::string                 serialAddr;
    pthread_t                   ipcThread;
    int                         serialFile;
    bool                        isOpen;
};


#endif
