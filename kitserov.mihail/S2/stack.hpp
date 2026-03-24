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
    void push(T& rhs)
    {
      data_.insert_tail(rhs);
    }

    T drop()
    {
      try {
        T value = data_.back();
        data_.pop_back();
        return value;
      } catch (const std::out_of_range&) {
        throw std::out_of_range("Stack is empty");
      }
    }
    T peek() const noexcept
    {
      return data_.back();
    }

    bool isEmpty()
    {
      return !(data_.get_size());
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