#ifndef STACK_HPP
#define STACK_HPP

template < class T >
class Stack {
  List< T > data_;
public:
  void push(const T& rhs)
  {
    try {
      data_.add(rhs);
    } catch (const std::out_of_range& e) {
      throw std::out_of_range("Failed to push element to stack");
    }
  }
  const T& drop()
  {
    try {
      const T& front = data_.front();
      data_.pop_front();
      return front;
    } catch (const std::out_of_range& e) {
      throw std::out_of_range("Stack is empty");
    }
  }
};

#endif