#include "scsp.hpp"
#include "tests.hpp"

auto main(int argc, char** argv) -> int {
    IntQueue queue;

    pthread_t writer, reader;

    pthread_create(&writer, nullptr, reinterpret_cast<void*(*)(void*)>(&writerThread), &queue);
    pthread_create(&reader, nullptr, reinterpret_cast<void*(*)(void*)>(&readerThread), &queue);

    pthread_join(writer, nullptr);
    pthread_join(reader, nullptr);
    return 0;
}