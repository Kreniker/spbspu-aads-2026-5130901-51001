#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include "list.hpp"

namespace kitserov
{
  template< class T >
  void printList(List< T >& list)
  {
    size_t s = list.get_size();
    for (size_t i = 0; i < s; ++i) {
      std::cout << *(list[i]);
      if (i + 1 != s) {
        std::cout << " ";
      }
    }
    std::cout << "\n";
  }

  template< class T >
  void clearListOfLists(List< List< T > >& listOfLists)
  {
    for (LIter< List< T > > it = listOfLists.begin(); it != listOfLists.end(); ++it) {
      it->clear();
    }
    listOfLists.clear();
  }
}

#endif
