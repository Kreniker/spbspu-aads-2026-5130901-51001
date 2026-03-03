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
    struct Node {
      T data;
      Node* next;
      Node(T v, Node* n = nullptr) : data(v), next(n) {}
    };
    Node* head;
    size_t size;
  public:
    List() : head(nullptr), size(0) {}
    ~List()
    {
      clear();
    }
    LIter< T > begin() 
    {
      return LIter< T >(head);
    }
    LIter< T > end()
    {
      return LIter< T >(nullptr);
    }
    List< T >* add(T v)
    {
      List< T >* newNode = new List< T >(v, this);
      size++;
      return newNode;
    }
    List< T >* insert(T v)
    {
      List< T >* newNode = next->add(v);
      next = newNode;
      return newNode;
    }
    T& front()
    {
      if (!size)
        throw std::out_of_range("list is empty");
      return head->data;
    }
  };


}

int main()
{
  using namespace kitserov;
  size_t names_cap = 8;
  size_t names_count = 0;
  std::string* names = new std::string[names_cap];
  List< List < int > > list_of_lists;
  while (true) {
    std::string name;
    if (!(std::cin >> name)) {
      break;
    }
    names[names_count] = name;
    names_count++;
    if (names_count >= names_cap) {
      names_cap *= 2;
      std::string* tmp = new std::string[names_cap];
      for (size_t i = 0; i < names_count; ++i) {
        tmp[i] = names[i];
      }
      delete[] names;
      names = tmp;
    }
    while (true) {
      
    }
  }
  std::cout << "\n";
  for (size_t i = 0; i < names_count; ++i) {
    std::cout << names[i] << " ";
  }
  return 0;
}
