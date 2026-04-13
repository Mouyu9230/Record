#include <iostream>
#include <queue>
#include <cmath>
#include <thread>
#include <vector>
#include <condition_variable>
#include <mutex>

using namespace std;

typedef struct thread_pool{
    vector<thread> threads;
    queue<int> task;

    condition_variable cv;
    mutex mtx;

}thread_pool;

void pool_init(thread_pool* pool){
    for(int i=0;i<10;i++){
        pool->threads.emplace_back(worker,pool);
    }
}

void get_task(){

}

void submit_task(){

}

void worker(thread_pool* pool){


    
}




int main(){

    thread_pool pool;
    pool_init(&pool);



}