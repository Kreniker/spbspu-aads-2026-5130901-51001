#ifndef STACK_HPP
#define STACK_HPP

#include <stdexcept>
#include "common/list.hpp"

namespace kitserov
{
  template< class T >
  class Stack
  {
  public:
    void push(const T& rhs)
    {
      data_.insert_tail(rhs);
    }

    void push(T&& rhs)
    {
      data_.insert_tail(std::move(rhs));
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

    const T& peek() const
    {
      return data_.back();
    }

    bool isEmpty() const noexcept
    {
      return data_.get_size() == 0;
    }

    void swap(Stack& other) noexcept
    {
      std::swap(data_, other.data_);
    }

    Stack(const Stack& other) :
      data_(other.data_)
    {}

    Stack(Stack&& other) noexcept :
      data_(std::move(other.data_))
    {}

    Stack& operator=(const Stack& other)
    {
      if (this != &other) {
        data_ = other.data_;
      }
      return *this;
    }

    Stack& operator=(Stack&& other) noexcept
    {
      if (this != &other) {
        data_ = std::move(other.data_);
      }
      return *this;
    }

    void clear()
    {
      data_.clear();
    }

    ~Stack() = default;

  private:
    List< T > data_;
  };
}

#endif