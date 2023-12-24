#include "scsp.hpp"
#include "tests.hpp"

auto main(int argc, char** argv) -> int {
    IntQueue queue;
    WriterTests::writer_test(queue, 2, 10);
    Utils::clear(queue);
    ReaderTests::reader_test(queue, 2, 10);
    return 0;
}