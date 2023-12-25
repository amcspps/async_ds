#include "scsp.hpp"
#include "tests.hpp"

auto main(int argc, char** argv) -> int {
    IntQueue queue;
    if (argc != 3) {
        std::cerr << "usage: " << argv[0] << " <thread count> <values per operator>" << std::endl;
        return 1;
    }

    int thread_count = std::atoi(argv[1]);
    int values_per_operator = std::atoi(argv[2]);
    
    if (thread_count == 0|| values_per_operator == 0) {
        std::cerr << "invalid arguments (must be not null)" << std::endl;
        return 1; 
    }
    
    WriterTests::writer_test(queue, thread_count, values_per_operator); 
    Utils::clear(queue);
    ReaderTests::reader_test(queue, thread_count, values_per_operator); 
    Utils::clear(queue);
    CommonTests::common_test(queue);
    return 0;
}