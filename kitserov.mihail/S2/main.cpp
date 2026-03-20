#include <fstream>
#include <iostream>
#include "queue.hpp"
#include "stack.hpp"
#include "utils.hpp"

int main(int argc, char* argv[])
{
    using namespace kitserov;
    if (argc > 2) {
        std::cerr << "Too many arguments\n";
        return 1;
    }
    bool haveFilename = argc == 2;

    std::ifstream file;
    std::istream& inputStream = std::cin;
    if (haveFilename) {
        file.open(argv[1]);
        if (!file.is_open()) {
            std::cerr << "Failed to open file: " << argv[1] << "\n";
            return 1;
        }
        inputStream = file;
    }

    bool isEof = false;
    while (!isEof) {
        Queue< std::string > tokens = readLine< std::string >(inputStream, isEof);
        (void)tokens;
    }
    return 0;
}