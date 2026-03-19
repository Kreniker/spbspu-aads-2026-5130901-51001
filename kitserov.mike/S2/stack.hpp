#ifndef STACK_HPP
#define STACK_HPP

#include "common/list.hpp"

template< class T >
class Stack {
  kitserov::List< T > data_;
public:
  void push(const T& rhs)
  {
    data_.add(rhs);
  }
  T drop()
  {
    try {
      T value = data_.front();
      data_.pop_front();
      return value;
    } catch (const std::out_of_range& e) {
      throw std::out_of_range("Stack is empty");
    }
  }
};

#endif