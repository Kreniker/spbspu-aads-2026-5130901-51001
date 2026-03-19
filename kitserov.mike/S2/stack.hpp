#ifndef STACK_HPP
#define STACK_HPP

template < class T >
struct Stack {
  void push(T rhs);
  T drop();
};

#endif