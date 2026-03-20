#ifdef STACK_HPP
#define STACK_HPP

namespace kitserov {

 template < class T >
 class Stack {
    List< T > data_;
  public:
    void push(T& rhs)
    {
      data_.add(rhs);
    }
    T& drop()
    {
      try {
        T& value = data_.front();
        data_.pop_front();
        return value;
      } catch (const std::out_of_range& e) {
        throw std::out_of_range("Stack is empty");
      }
    }
  };
}

#endif