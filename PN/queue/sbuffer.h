
#include "isbuffer.h"
#include<queue>
#include<mutex>
#include<condition_variable>




template <class ItemType, unsigned N > 
class SBuffer:public ISBuffer<ItemType,SBuffer<ItemType,N> >{
    std::condition_variable cv_used;
    std::condition_variable cv_queue;
    std::mutex m_used;
    std::mutex m_queue;
    std::queue<ItemType*> used;
    std::queue<ItemType*> queue;
    bool stop_flag = false;

    public:
        SBuffer();

        int enqueue(ItemType * pit);
        ItemType * dequeue();

        ItemType * allocItem();
        void freeItem(ItemType * pit);

        void stop();
};

template <class ItemType, unsigned int N >
SBuffer<ItemType,N>::SBuffer(){
    for(int i = 0; i < N; i++)
        used.push(new ItemType());
}

template <class ItemType, unsigned int N >
int SBuffer<ItemType,N>::enqueue(ItemType * pit){
    if(stop_flag)
        return 0;
    std::unique_lock<std::mutex> lock(m_queue);
    queue.push(pit);
    lock.unlock();
    cv_queue.notify_one();
    return 1;
}

template <class ItemType, unsigned int N >
ItemType * SBuffer<ItemType,N>::dequeue(){
    std::unique_lock<std::mutex> lock(m_queue);
    cv_queue.wait(lock, [&]{return !queue.empty() || stop_flag;});
    if(queue.empty())
        return nullptr;
    ItemType *ptr = queue.front();
    queue.pop();

    return ptr;
}

template <class ItemType, unsigned int N >
ItemType * SBuffer<ItemType,N>::allocItem(){
    std::unique_lock<std::mutex> lock(m_used);
    cv_used.wait(lock, [&]{return !used.empty() || stop_flag;});
    if(stop_flag)
        return nullptr;
    ItemType *ptr = used.front();
    used.pop();

    return ptr;
}

template <class ItemType, unsigned int N >
void SBuffer<ItemType,N>::freeItem(ItemType * pit){
    std::unique_lock<std::mutex> lock(m_used);
    used.push(pit);
    lock.unlock();
    cv_used.notify_one();
}

template <class ItemType, unsigned int N >
void SBuffer<ItemType,N>::stop(){
    stop_flag = true;
    cv_queue.notify_all();
    cv_used.notify_all();
}

