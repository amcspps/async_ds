#include "tests.hpp"
#include <algorithm>
#include <atomic>

std::ostream& operator<<(std::ostream& os, const std::vector<int>& vec) {
    os << "[ ";
    for (const auto& element : vec) {
        os << element << ' ';
    }
    os << "]";
    return os;
}


void Utils::remove_duplicates(std::vector<int>& vec) {
    std::sort(vec.begin(), vec.end());
    auto last = std::unique(vec.begin(), vec.end());
    vec.erase(last, vec.end());    
}

void Utils::fill(IntQueue& queue, std::vector<int>& reference, int threads_num) {
    for (int i = 0; i < threads_num; ++i) {
        for (auto el: reference) {
            queue.enqueue(el);
        }
    }   
}

void Utils::compare(std::vector<int>& reference, std::vector<int>& output) {
    bool status = std::equal(reference.begin(), reference.end(), output.begin(), output.end());
    if (status) {
        std::cout << "vectors are equal." << std::endl;

    } else {
        std::cout << "vectors are not equal." << std::endl;
        for (auto el: output) {
            std::cout << el << " ";
        }
        std::cout << std::endl;
    }
}


void Utils::fill(std::vector<int>& reference, int count) {
    for (int i = 0; i < count; i++) {
        reference.push_back(i);
    }
    std:: cout << "the reference is: ";
    for (auto el: reference) {
        std::cout << el << " ";
    }
    std::cout << std::endl;
}

void Utils::clear(IntQueue &queue) {
    if (queue.is_empty()) {
        std::cout << "empty!" << std::endl;
    }
    else {
        while (!queue.is_empty()) {
            int value = queue.dequeue();
        }
    }
}
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
    WriterArgs* thread_args = static_cast<WriterArgs*>(args);
    IntQueue& queue = *(thread_args->queue);
    int values_per_writer = thread_args->values_per_writer;
    for (int i = 0; i < values_per_writer; ++i) {
        int status = queue.enqueue(i);
    }
    return nullptr;
}

void WriterTests::writer_test(IntQueue &queue, int num_writers, int values_per_writer) {
    std::vector<pthread_t> writer_threads;
    WriterArgs thread_args{&queue, values_per_writer};
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

void* ReaderTests::reader_thread(void* args) {
    ReaderArgs* thread_args = static_cast<ReaderArgs*>(args);
    IntQueue& queue = *(thread_args->queue);
    int values_per_reader = thread_args->values_per_reader;
    auto thread_data =  thread_args->thread_out_data;
    for (int i = 0; i < values_per_reader; ++i) {
        thread_data->push_back(queue.dequeue());
    }
    return nullptr;
}

void ReaderTests::reader_test(IntQueue &queue, int num_readers, int values_per_reader) {
    std::vector<int> reference;
    Utils::fill(reference, values_per_reader);
    Utils::fill(queue, reference, num_readers);

    std::vector<pthread_t> reader_threads;
    std::vector<ReaderArgs> thread_args_list;

    for(int i = 0; i < num_readers; ++i) {
        thread_args_list.push_back(ReaderArgs{&queue, values_per_reader, std::make_shared<std::vector<int>>()});
    }

    for (int i = 0; i < num_readers; ++i) {
        pthread_t thread;
        pthread_create(&thread, nullptr, reinterpret_cast<void*(*)(void*)>(&reader_thread), &thread_args_list[i]);
        reader_threads.push_back(thread);
    }

    for (auto& thread : reader_threads) {
        pthread_join(thread, nullptr);
    }
    std::vector<int> output;
    for (const auto& arg : thread_args_list) {
        output.insert(output.end(), arg.thread_out_data->begin(), arg.thread_out_data->end());
    }
    Utils::remove_duplicates(output);
    std::cout << reference << std::endl;
    std::cout << output << std::endl;
    Utils::compare(reference, output);

}




void* CommonTests::reader_thread(void* args) {
    
    CommonArgs* thread_args = reinterpret_cast<CommonArgs*>(args);
    IntQueue& queue = *(thread_args->queue);
    int n = thread_args->n;
    auto ver_arr = thread_args->verification_array;

    for (int i = 0; i < n; ++i) {
        int value = queue.dequeue();
        (ver_arr->data()[value])++;
    }
    return nullptr;
}
 
void* CommonTests::writer_thread(void* args) {
    CommonArgs* thread_args = reinterpret_cast<CommonArgs*>(args);
    IntQueue& queue = *(thread_args->queue);
    int n = thread_args->n;
    auto ver_arr = thread_args->verification_array;

    for (int i = 0; i < n; ++i) {
        queue.enqueue(i);
    }
    return nullptr;
}



void CommonTests::common_test(IntQueue &queue) {
    const int n = queue.get_size();
    auto verification_array = std::make_shared<std::vector<int>>(n, 0);
    pthread_t reader, writer;
    CommonArgs thread_args {&queue, n, verification_array};

    pthread_create(&writer, nullptr, reinterpret_cast<void*(*)(void*)>(&writer_thread), &thread_args);
    pthread_create(&reader, nullptr, reinterpret_cast<void*(*)(void*)>(&reader_thread), &thread_args);

    pthread_join(writer, nullptr);
    pthread_join(reader, nullptr);

    for (int i = 0; i < n; ++i) {
        if (verification_array->data()[i] != 1) {
            std::cout << "verification failed for element: " << i << std::endl;
        }
    }
    std::cout << "verification passed!" << std::endl;
}