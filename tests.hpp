#include "scsp.hpp"
#include <vector>
#include <memory>
#ifndef TESTS_HPP
#define TESTS_HPP

struct WriterArgs {
    IntQueue* queue;
    int values_per_writer;
};

struct ReaderArgs {
    IntQueue* queue;
    int values_per_reader;
    std::shared_ptr<std::vector<int>> thread_out_data;
};

struct CommonArgs {
    IntQueue* queue;
    int n;
    std::shared_ptr<std::vector<int>> verification_array;
};

namespace Utils {
    int check(IntQueue& queue);
    void clear(IntQueue& queue);
    void fill(std::vector<int>& reference, int count);
    void fill(IntQueue& queue, std::vector<int>& reference, int threads_num);
    void print(const std::vector<int> vec);
    void compare(std::vector<int>& reference, std::vector<int>& output);
    void remove_duplicates(std::vector<int>& vec);
};

namespace WriterTests {
    void* writer_thread(void* args);
    void writer_test(IntQueue &queue, int num_writers, int values_per_writer);
};

namespace ReaderTests {
    void* reader_thread(void* args);
    void reader_test(IntQueue &queue, int num_writers, int values_per_writer);
};
namespace CommonTests {
    void* reader_thread(void* args);
    void* writer_thread(void* args);
    void common_test(IntQueue &queue);
}
std::ostream& operator<<(std::ostream& os, const std::vector<int>& vec);
#endif