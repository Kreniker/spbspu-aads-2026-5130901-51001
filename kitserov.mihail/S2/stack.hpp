#ifndef STACK_HPP
#define STACK_HPP

#include <stdexcept>
#include "common/list.hpp"

namespace kitserov
{
  template< class T >
  class Stack
  {
    List< T > data_;
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
      } catch (const std::out_of_range&) {
        throw std::out_of_range("Stack is empty");
      }
    }

    void clear()
    {
      data_.clear();
    }
    ~Stack()
    {
      clear();
    }
  };
}

#endif