#include "scsp.hpp"
#include <vector>
#ifndef UTILS_HPP
#define UTILS_HPP
const int HALF = 10;
const int ENTIRE = 20;

struct ThreadArgs {
    IntQueue* queue;
    int values_per_writer;
};


namespace Utils {
    int check(IntQueue &queue);
}

namespace WriterTests {
    void* writer_thread(void* args);
    void writer_test(IntQueue &queue, int num_writers, int values_per_writer);
}

namespace ReaderTests {
    void reader_thread(IntQueue &queue);
}
#endif