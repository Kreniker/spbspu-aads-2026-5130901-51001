#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <string>
#include "queue.hpp"

namespace kitserov
{
  template< class T >
  Queue< T > readLine(std::istream& in, bool& isEof)
  {
    isEof = false;
    Queue< T > queue;
    T token;
    bool isSkipws = in.flags() & std::ios_base::skipws;
    if (!isSkipws) {
      in >> std::skipws;
    }
    while (true) {
      const int nextChar = in.peek();
      if (nextChar == EOF || nextChar == '\n') {
        if (nextChar == EOF) {
          isEof = true;
        }
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