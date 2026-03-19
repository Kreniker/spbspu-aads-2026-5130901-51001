#ifndef STACK_HPP
#define STACK_HPP

template < class T >
class Stack {
  List< T > data_;
public:
  void push(const T& rhs);
  const T& drop();
};

#endif