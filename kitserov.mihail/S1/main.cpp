#include "list.hpp"
#include <limits>
#include <iostream>
#include <string>

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

int main()
{
  using namespace kitserov;
  List< std::string > names;
  List< List< int > > list_of_lists;
  std::string name;
  while (true) {
    try {
      if (!(std::cin >> name)) {
        break;
      }
      names.insert_tail(name);
      List< int > numbers;
      int num;
      while (std::cin >> num) {
        numbers.insert_tail(num);
      }
      if (std::cin.eof()) {
        list_of_lists.insert_tail(numbers);
        numbers.clear();
        break;
      }
      if (std::cin.fail()) {
        std::cin.clear();
        std::cin >> std::ws;
        int next_char = std::cin.peek();
        if (next_char != EOF && std::isdigit(static_cast< unsigned char >(next_char)))
        {
          std::cerr << "Overflow\n";
          numbers.clear();
          names.clear();
          clear_list_of_lists(list_of_lists);
          return 1;
        }
        list_of_lists.insert_tail(numbers);
      }
      numbers.clear();
    } catch(...) {
      names.clear();
      clear_list_of_lists(list_of_lists);
      std::cerr << "bad allocated memory\n";
      return 2;
    }
  }
  if (names.get_size() == 0) {
    std::cout << "0\n";
    return 0;
  }
  std::cout << "\n";
  print_list(names);
  size_t max_size = 0;
  for (LIter< List< int > > it = list_of_lists.begin(); it != list_of_lists.end(); ++it) {
    size_t s = it->get_size();
    if (s > max_size) {
      max_size = s;
    }
  }
  List< int > summes;
  for (size_t i = 0; i < max_size; i++) {
    int sum = 0;
    for (size_t j = 0; j < list_of_lists.get_size(); j++) {
      List< int >& curList = *(list_of_lists[j]);
      if (curList.get_size() > i) {
        int val = *(curList[i]);
        std::cout << val << " ";
        if (sum > std::numeric_limits< int >::max() - val) {
          std::cerr << "Overflow sum\n";
          summes.clear();
          names.clear();
          clear_list_of_lists(list_of_lists);
          return 1;
        }
        sum += val;
      }
    }
    summes.insert_tail(sum);
    std::cout << "\n";
  }
  print_list(summes);
  summes.clear();
  names.clear();
  clear_list_of_lists(list_of_lists);
  return 0;
}
