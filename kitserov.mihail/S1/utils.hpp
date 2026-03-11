#ifndef UTILS_HPP
#define UTILS_HPP

#include "list.hpp"
#include <iostream>

namespace kitserov
{
  template< class T >
  void print_list(List< T >& list)
  {
    for (LIter< T > it = list.begin(); it != list.end(); ++it) {
      std::cout << *it << " ";
    }
    std::cout << "\n";
  }
  template< class T >
  void clear_list_of_lists(List< List< T > >& list_of_lists)
  {
    for (LIter< List< T > > it = list_of_lists.begin(); it != list_of_lists.end(); ++it) {
      (*it).clear();
    }
    list_of_lists.clear();
  }
}

#endif
