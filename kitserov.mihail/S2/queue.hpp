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
    void push(T& rhs)
    {
      data_.insert_tail(rhs);
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
    bool isEmpty()
    {
      return !(data_.get_size());
    }
    void swap(Queue& other) noexcept
    {
      std::swap(data_, other.data_);
    }
    void operator()(Queue& other)
    {
      (*this).swap(other);
    }
    Queue& operator=(const Queue& other)
    {
      if (data_ != &other.data_) {
        List tmp(other);
        swap(tmp);
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
  private:
    List< T > data_;
  };
}


#endif