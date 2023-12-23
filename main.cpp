#include "scsp.hpp"
#include "tests.hpp"

auto main(int argc, char** argv) -> int {
    IntQueue queue;
    WriterTests::writer_test(queue, 2, 25);
    return 0;
}