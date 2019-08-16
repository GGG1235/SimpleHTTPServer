//
// Created by GGG1235 on 2019-08-15.
//
#ifndef SEMAPHORE
#define SEMAPHORE

#include <mutex>
#include <condition_variable>

namespace mthd{
    class semaphore{
    public:
        semaphore(int value=1):count(value),wakeups(0){}
        void wait(){
            std::unique_lock<std::mutex> lock(mutex);
            if (--count<0){
                condition.wait(lock,[&]()-> bool{return wakeups>0;});
                --wakeups;
            }
        }
        void signal(){
            std::lock_guard<std::mutex> lock(mutex);
            if (++count<=0){
                ++wakeups;
                condition.notify_one();
            }
        }

    private:
        int count;
        int wakeups;
        std::mutex mutex;
        std::condition_variable condition;
    };
};
#endif //SEMAPHORE