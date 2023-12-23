#include "tests.hpp"

int Utils::check(IntQueue &queue) {
    int dequeued = 0;
    if (queue.is_empty()) {
        std::cout << "empty!" << std::endl;
        return 0;
    }
    else {
        std::cout << "not empty, dequeued: ";
        while (!queue.is_empty()) {
            int value = queue.dequeue();
            std::cout << value << " ";
            dequeued++;
        }
        return dequeued;
        std::cout << std::endl;
    }
}

void* WriterTests::writer_thread(void* args) {
    ThreadArgs* thread_args = static_cast<ThreadArgs*>(args);
    IntQueue& queue = *(thread_args->queue);
    int values_per_writer = thread_args->values_per_writer;
    for (int i = 0; i < values_per_writer; ++i) {
        int status = queue.enqueue(i);
    }
    return nullptr;
}

void WriterTests::writer_test(IntQueue &queue, int num_writers, int values_per_writer) {
    std::vector<pthread_t> writer_threads;
    ThreadArgs thread_args{&queue, values_per_writer};
    //start
    for (int i = 0; i < num_writers; ++i) {
        pthread_t thread;
        pthread_create(&thread, nullptr, reinterpret_cast<void*(*)(void*)>(&writer_thread), &thread_args);
        writer_threads.push_back(thread);
    }
    //wait for writers finish
    for (auto& thread : writer_threads) {
        pthread_join(thread, nullptr);
    }
    if (Utils::check(queue) == num_writers * values_per_writer) {
        std::cout << "OK" << std::endl;
    }
    else {
        std::cout << "NOT OK" <<std::endl;
    }    
}

void ReaderTests::reader_thread(IntQueue &queue) {
    for (int i = 0; i < ENTIRE; ++i) {
        int value = queue.dequeue();
    }
}



