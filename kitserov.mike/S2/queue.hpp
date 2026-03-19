#ifndef QUEUE_HPP
#define QUEUE_HPP

template < class T >
struct Queue {
  void push(T rhs);
  T drop();
};

#endif