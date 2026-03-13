#include <limits>
#include <iostream>
#include <string>
#include "list.hpp"
#include "utils.hpp"

int main()
{
  using namespace kitserov;
  List< std::string > names;
  List< List< int > > listOfLists;
  std::string name;
  bool hasNumbers = false;
  while (true) {
    try {
      if (!(std::cin >> name)) {
        break;
      }
      names.insert_tail(name);
      List< int > numbers;
      size_t value;
      while (true) {
        if(!(std::cin >> value)) {
          if (std::cin.eof()) break;
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
        if (value > std::numeric_limits< int >::max()) {
          std::cerr << "Overflow\n";
          numbers.clear();
          names.clear();
          clearListOfLists(listOfLists);
          return 1;
        }
        int intValue = static_cast< int >(value);
        numbers.insert_tail(intValue);
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
  //std::cout << "\n";
  if (names.get_size() == 0 || !hasNumbers) {
    printList(names);
    std::cout << "0\n";
    names.clear();
    return 0;
  }
  printList(names);
  size_t maxSize = 0;
  for (LIter< List< int > > it = listOfLists.begin(); it != listOfLists.end(); ++it) {
    size_t s = it->get_size();
    if (s > maxSize) {
      maxSize = s;
    }
  }
  List< int > summes;
  for (size_t i = 0; i < maxSize; i++) {
    int sum = 0;
    for (size_t j = 0; j < listOfLists.get_size(); j++) {
      List< int >& curList = *(listOfLists[j]);
      if (curList.get_size() > i) {
        int val = *(curList[i]);
        std::cout << val << " ";
        if (sum > std::numeric_limits< int >::max() - val) {
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
