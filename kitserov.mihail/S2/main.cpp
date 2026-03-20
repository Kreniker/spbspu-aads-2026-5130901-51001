#include <fstream>
#include <iostream>
#include "queue.hpp"
#include "stack.hpp"

int main(int argc, char* argv[])
{
    using namespace kitserov;
    if (argc > 2) {
        std::cerr << "Too many arguments\n";
        return 1;
    }
    bool haveFilename = argc == 2;
    if (haveFilename) {
        std::ifstream file(argv[1]);
        if (!file.is_open()) {
            std::cerr << "Failed to open file: " << argv[1] << "\n";
            return 1;
        }
    }
}