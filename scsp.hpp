#include <iostream>
#include <list>
#include <pthread.h>
#include <ctime>
#ifndef SCSP_HPP
#define SCSP_HPP
class IntQueue {
public:
    IntQueue() {
        if (pthread_mutex_init(&read_mutex_, nullptr) != 0) {
            std::cerr << "error initializing read mutex." << std::endl;
            // TODO: handle error
        }
        if (pthread_mutex_init(&write_mutex_, nullptr) != 0) {
            std::cerr << "error initializing write mutex." << std::endl;
            pthread_mutex_destroy(&read_mutex_);
            // TODO: handle error

        }
        if (pthread_cond_init(&not_empty_, nullptr) != 0 || pthread_cond_init(&not_full_, nullptr) != 0) {
            std::cerr << "error initializing condition variables." << std::endl;
            pthread_mutex_destroy(&read_mutex_);
            pthread_mutex_destroy(&write_mutex_);
            // TODO: handle error
        }
    }

    ~IntQueue() {
        pthread_mutex_destroy(&read_mutex_);
        pthread_mutex_destroy(&write_mutex_);
        pthread_cond_destroy(&not_empty_);
        pthread_cond_destroy(&not_full_);
    }

    int enqueue(int value) {
        pthread_mutex_lock(&write_mutex_);

        struct timespec ts;
        clock_gettime(CLOCK_REALTIME, &ts);
        ts.tv_sec += timeout_s;
        ts.tv_nsec = 0;

        while (queue_.size() == MAX_QUEUE_SIZE) {
            if (pthread_cond_timedwait(&not_full_, &write_mutex_, &ts) != 0) {
                // timeout occurred
                std::cerr << "insertion timeout" << std::endl;
                pthread_mutex_unlock(&write_mutex_);
                return -1; // failure
            }
        }

        if (queue_.size() < MAX_QUEUE_SIZE) {
            queue_.push_back(value);
            pthread_cond_signal(&not_empty_);
            pthread_mutex_unlock(&write_mutex_);
            return 0; // success
        } else {
            std::cerr << "error: queue is full." << std::endl;
            pthread_mutex_unlock(&write_mutex_);
            return -1; // failure
        }
    }

    int dequeue() {
        pthread_mutex_lock(&read_mutex_);

        struct timespec ts;
        clock_gettime(CLOCK_REALTIME, &ts);
        ts.tv_sec += timeout_s;
        ts.tv_nsec = 0;

        while (queue_.empty()) {
            if (pthread_cond_timedwait(&not_full_, &write_mutex_, &ts) != 0) {
                // timeout occurred
                std::cerr << "reading timeout (empty)" << std::endl;
                pthread_mutex_unlock(&write_mutex_);
                return -1; // failure
            }
        }

        if (!queue_.empty()) {
            int value = queue_.front();
            queue_.pop_front();
            pthread_cond_signal(&not_full_);
            pthread_mutex_unlock(&read_mutex_);
            return value; // success
        } else {
            std::cerr << "error: queue is empty." << std::endl;
            pthread_mutex_unlock(&read_mutex_);
            return -1; // failure
        }
    }
    bool is_empty()  {
        pthread_mutex_lock(&read_mutex_);
        bool empty = queue_.empty();
        pthread_mutex_unlock(&read_mutex_);
        return empty;
    }
    int get_size() const {
        return MAX_QUEUE_SIZE;
    }
private:
    std::list<int> queue_;
    pthread_mutex_t read_mutex_;
    pthread_mutex_t write_mutex_;
    pthread_cond_t not_empty_;
    pthread_cond_t not_full_;
    static const size_t MAX_QUEUE_SIZE = 30;
    int timeout_s = 1;
};
#endif