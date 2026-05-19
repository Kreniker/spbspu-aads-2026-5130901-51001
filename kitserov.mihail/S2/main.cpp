#include <fstream>
#include <iostream>
#include <string>
#include <stdexcept>
#include "queue.hpp"
#include "stack.hpp"
#include "utils2.hpp"

int main(int argc, char* argv[])
{
  using namespace kitserov;

  if (argc > 2) {
    std::cerr << "Too many arguments\n";
    return 1;
  }

  bool haveFilename = (argc == 2);

  std::ifstream file;
  std::istream* inputStream = &std::cin;
  if (haveFilename) {
    file.open(argv[1]);
    if (!file.is_open()) {
      std::cerr << "Failed to open file: " << argv[1] << "\n";
      return 1;
    }
    inputStream = &file;
  }

  Stack< long long > results;
  bool isEof = false;

  while (!isEof) {
    try {
      Queue< std::string > tokens = readLine< std::string >(*inputStream, isEof);
      if (tokens.isEmpty()) {
        continue;
      }
      Queue< std::string > postfixTokens = infixToPostfix< std::string >(tokens);
      long long tmp = calculatePostfix< long long >(postfixTokens);
      results.push(tmp);
    } catch (const std::exception& e) {
      std::cerr << "Error: " << e.what() << "\n";
      return 1;
    } catch (...) {
      std::cerr << "Unknown exception\n";
      return 1;
    }
  }

  bool isFirst = true;
  while (!results.isEmpty()) {
    if (!isFirst) {
      std::cout << " ";
    }
    std::cout << results.drop();
    isFirst = false;
  }
  std::cout << "\n";
  return 0;
}
