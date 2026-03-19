#ifndef QUEUE_HPP
#define QUEUE_HPP

template < class T >
class Queue {
  kitserov::List< T > data_;
public:
  void push(const T& rhs)
  {
    try {
      data_.add(rhs);
    } catch (const std::out_of_range& e) {
      throw std::out_of_range("Failed to push element to queue");
    }
  }
  const T& drop()
  {
    try {
      const T& back = data_.back();
      data_.pop_back();
      return back;
    } catch (const std::out_of_range& e) {
      throw std::out_of_range("Queue is empty");
    }
  }
};