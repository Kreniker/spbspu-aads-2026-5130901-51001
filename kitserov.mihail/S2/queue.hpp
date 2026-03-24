#ifndef QUEUE_HPP
#define QUEUE_HPP

#include <stdexcept>
#include "common/list.hpp"
#include "common/utils.hpp"

namespace kitserov
{
  template< class T >
  class Queue
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
        T value = data_.front();
        data_.pop_front();
        return value;
      } catch (const std::out_of_range&) {
        throw std::out_of_range("Queue is empty");
      }
    }

    bool isEmpty() const noexcept
    {
      return data_.get_size() == 0;
    }

    void swap(Queue& other) noexcept
    {
      std::swap(data_, other.data_);
    }

    Queue& operator=(const Queue& other)
    {
      if (this != &other) {
        Queue tmp(other);
        swap(tmp);
      }
      return *this;
    }

    Queue& operator=(Queue&& other) noexcept
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

    List< T >& inList()
    {
      return data_;
    }

    const List< T >& inList() const
    {
      return data_;
    }

  private:
    List< T > data_;
  };
}

#endif