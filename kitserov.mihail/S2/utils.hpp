#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <string>
#include "queue.hpp"

namespace kitserov
{
  Queue< std::string > readLine(std::istream& in)
  {
    Queue< std::string > queue;
    std::string token;
    bool isSkipws = in.flags() & std::ios_base::skipws;
    if (!isSkipws) {
      in >> std::skipws;
    }
    while (true) {
      const int nextChar = in.peek();
      if (nextChar == EOF || nextChar == '\n') {
        break;
      }

      if (!(in >> token)) {
        break;
      }

      queue.push(token);
    }
    if (!isSkipws) {
        in >> std::noskipws;
    }
    return queue;
  }
}

#endif