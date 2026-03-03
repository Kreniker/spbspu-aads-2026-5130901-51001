#include <iostream>
#include <string>

namespace kitserov
{
  template< class T > class List;
  template < class T >
  class LIter
  {
    friend class List< T >;
    List< T >* node;
    LIter(List< T >* n) : node(n) {}
  public:
    LIter() : node(nullptr) {}
    T& operator*() const
    {
      return node->data;
    }
    T* operator->() const
    {
      return &node->data;
    }
    LIter& operator++()
    {
      if (node) {
      	node = node->next;
      }
      return *this;
    }
    bool operator==(const LIter& other) const
    {
      return node == other.node;
    }
    bool operator!=(const LIter& other) const
    {
      return node != other.node;
    }
  };

  template< class T >
  class LCIter
  {
    friend class List< T >;
    
  };

  template< class T >
  class List
  {
  public:
    T data;
    List< T >* next;
    size_t size;

    List(T v, List< T >* n) : data(v), next(n) {}
    LIter< T > begin() 
    {
      return LIter< T >(this);
    }
    LIter< T > end()
    {
      return LIter< T >(nullptr);
    }
    List< T >* add(T v) {
      List< T >* newNode = new List< T >(v, this);
      return newNode;
    }
  };


}

int main()
{
  using namespace kitserov;
  size_t names_cap = 8;
  size_t names_count = 0;
  std::string* names = new std::string[names_cap];
  while (true) {
    std::string name;
    if (!(std::cin >> name)) {
      delete[] names;
      break;
    }
    if (names_count >= names_cap) {
      names_cap *= 2;
      std::string* tmp = new std::string[names_cap];
      for (size_t i = 0; i < names_count; ++i) {
        tmp[i] = names[i];
      }
      delete[] names;
      names = tmp;
    }
  }
}
