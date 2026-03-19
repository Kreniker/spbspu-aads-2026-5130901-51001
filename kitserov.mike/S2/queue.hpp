#ifndef QUEUE_HPP
#define QUEUE_HPP

#include "common/list.hpp"

template< class T >
class Queue {
  kitserov::List< T > data_;
public:
  void push(const T& rhs)
  {
    data_.add(rhs);
  }
  T drop()
  {
    try {
      T value = data_.back();
      data_.pop_back();
      return value;
    } catch (const std::out_of_range& e) {
      throw std::out_of_range("Queue is empty");
    }
  }
};

#endif