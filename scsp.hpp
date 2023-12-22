#include <iostream>
#include <list>
#include <pthread.h>

class IntQueue {
public:
    IntQueue() {
        pthread_mutex_init(&read_mutex_, nullptr);
        pthread_mutex_init(&write_mutex_, nullptr);
        pthread_cond_init(&not_empty_, nullptr);
        pthread_cond_init(&not_full_, nullptr);
    }

    ~IntQueue() {
        pthread_mutex_destroy(&read_mutex_);
        pthread_mutex_destroy(&write_mutex_);
        pthread_cond_destroy(&not_empty_);
        pthread_cond_destroy(&not_full_);
    }

    void enqueue(int value) {
        pthread_mutex_lock(&write_mutex_);

        while (queue_.size() == MAX_QUEUE_SIZE) {
            pthread_cond_wait(&not_full_, &write_mutex_);
        }

        queue_.push_back(value);
        pthread_cond_signal(&not_empty_);
        pthread_mutex_unlock(&write_mutex_);
    }

    int dequeue() {
        pthread_mutex_lock(&read_mutex_);

        while (queue_.empty()) {
            pthread_cond_wait(&not_empty_, &read_mutex_);
        }

        int value = queue_.front();
        queue_.pop_front();
        pthread_cond_signal(&not_full_);
        pthread_mutex_unlock(&read_mutex_);

        return value;
    }

private:
    std::list<int> queue_;
    pthread_mutex_t read_mutex_;
    pthread_mutex_t write_mutex_;
    pthread_cond_t not_empty_;
    pthread_cond_t not_full_;
    static const size_t MAX_QUEUE_SIZE = 10;
};

void writerThread(IntQueue &queue) {
    for (int i = 0; i < 20; ++i) {
        queue.enqueue(i);
        std::cout << "Enqueued: " << i << std::endl;
    }
}

void readerThread(IntQueue &queue) {
    for (int i = 0; i < 20; ++i) {
        int value = queue.dequeue();
        std::cout << "Dequeued: " << value << std::endl;
    }
}