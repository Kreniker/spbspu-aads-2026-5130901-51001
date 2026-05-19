#include <limits>
#include <iostream>
#include <string>
#include "list.hpp"
#include "utils.hpp"

int main()
{
  using namespace kitserov;
  List< std::string > names;
  List< List< size_t > > listOfLists;
  std::string name;
  bool hasNumbers = false;

  while (true) {
    try {
      if (!(std::cin >> name)) {
        break;
      }
      names.insert_tail(name);
      List< size_t > numbers;
      size_t value;

      while (true) {
        if (!(std::cin >> value)) {
          if (std::cin.eof()) {
            break;
          }
          std::cin.clear();
          std::cin >> std::ws;
          int nextChar = std::cin.peek();
          if (nextChar != EOF && std::isdigit(static_cast< unsigned char >(nextChar))) {
            std::cerr << "Overflow\n";
            numbers.clear();
            names.clear();
            clearListOfLists(listOfLists);
            return 1;
          }
          break;
        }
        numbers.insert_tail(value);
        hasNumbers = true;
      }
      listOfLists.insert_tail(numbers);
    } catch (...) {
      names.clear();
      clearListOfLists(listOfLists);
      std::cerr << "bad allocated memory\n";
      return 2;
    }
  }

  if (names.get_size() == 0 || !hasNumbers) {
    if (names.get_size() != 0) {
      printList(names);
    }
    std::cout << "0\n";
    names.clear();
    return 0;
  }

  printList(names);

  size_t maxSize = 0;
  for (LIter< List< size_t > > it = listOfLists.begin(); it != listOfLists.end(); ++it) {
    size_t s = it->get_size();
    if (s > maxSize) {
      maxSize = s;
    }
  }

  List< size_t > summes;
  for (size_t i = 0; i < maxSize; i++) {
    size_t sum = 0;
    bool first = true;
    for (size_t j = 0; j < listOfLists.get_size(); j++) {
      List< size_t >& curList = *(listOfLists[j]);
      if (curList.get_size() > i) {
        size_t val = *(curList[i]);
        if (!first) {
          std::cout << " ";
        }
        std::cout << val;
        first = false;
        if (sum > std::numeric_limits< size_t >::max() - val) {
          std::cout << "\n";
          std::cerr << "Overflow sum\n";
          summes.clear();
          names.clear();
          clearListOfLists(listOfLists);
          return 1;
        }
        sum += val;
      }
    }
    summes.insert_tail(sum);
    std::cout << "\n";
  }

  printList(summes);
  summes.clear();
  names.clear();
  clearListOfLists(listOfLists);
  return 0;
}
