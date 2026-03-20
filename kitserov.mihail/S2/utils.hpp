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
    while (true) {
      const int nextChar = in.peek();
      if (nextChar == EOF) {
          isEof = true;
        break;
      }
      if (nextChar == '\n') {
        in.get();
        break;
      }

      if (!(in >> token)) {
        break;
      }

      queue.push(token);
    }
    return queue;
  }
}

#endif