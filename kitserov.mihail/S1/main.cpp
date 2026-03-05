#include <iostream>
#include <string>

namespace kitserov
{
  template< class T > class List;
  template < class T >
  class LIter
  {
    friend class List< T >;
    typename List< T >::Node* node_;
  public:
    LIter() : node_(nullptr) {}
    LIter(typename List<T>::Node* n) : node_(n) {}
    T& operator*() const
    {
      return node_->data;
    }
    T* operator->() const
    {
      return &node_->data;
    }
    LIter& operator++()
    {
      if (node_) {
      	node_ = node_->next;
      }
      return *this;
    }
    bool operator==(const LIter& other) const
    {
      return node_ == other.node_;
    }
    bool operator!=(const LIter& other) const
    {
      return node_ != other.node_;
    }
  };

  template< class T >
  class LCIter
  {
    friend class List< T >;
    
  };

  template< class T >
  struct List
  {
  public:
    struct Node {
      T data;
      Node* next;
      Node(T&& v, Node* n = nullptr) : data(std::move(v)), next(n) {}
    };
    List() : head(nullptr), size(0) {}
    ~List()
    {
      clear();
    }
    List(const List&) = delete;
    List& operator=(const List&) = delete;
    List(List&& other) noexcept : head(other.head), size(other.size)
    {
      other.head = nullptr;
      other.size = 0;
    }
    List& operator=(List&& other) noexcept
    {
      if (this != &other) {
        clear();
        head = other.head;
        size = other.size;
        other.head = nullptr;
        other.size = 0;
      }
      return *this;
    }
    LIter< T > begin() 
    {
      return LIter< T >(head);
    }
    LIter< T > end()
    {
      return LIter< T >(nullptr);
    }
    LIter< T > operator[](size_t index)
    {
      if (index >= size) {
        throw std::out_of_range("out_of_range");
      }
      Node* current = head;
      for (size_t i = 0; i < index; i++) {
        current = current->next;
      }
      return LIter< T >(current);
    }
    Node* add(T&& v)
    {
      Node* newNode = new Node(std::move(v), head);
      head = newNode;
      size++;
      return newNode;
    }
    Node* insert(T&& v, LIter< T > pos)
    {
      size++;
      if (!pos.node_) {
        return add(std::move(v));
      }
      Node* newNode = new Node(std::move(v), pos.node_->next);
      pos.node_->next = newNode;
      return newNode;
    }
    Node* insert_tail(T&& v)
    {
      if (size == 0) {
        return add(std::move(v));
      }
      return insert(std::move(v), (*this)[size - 1]);
    }
    T& front()
    {
      if (!size) {
        throw std::out_of_range("list is empty");
      }
      return head->data;
    }
    T& back()
    {
      if (!size) {
        throw std::out_of_range("list is empty");
      }
      return *((*this)[size - 1]);
    }
    void clear(LIter< T > from, LIter< T > to)
    {
      if (from.node_ == nullptr) {
        return;
      }
      if (head == from.node_) {
        Node* newHead = to.node_;
        Node* current = head;
        while (current != to.node_) {
          Node* tmp = current->next;
          delete current;
          current = tmp;
          --size;
        }
        head = newHead;
        return;
      }
      Node* prev = head;
      while (prev != nullptr && prev->next != from.node_) {
        prev = prev->next;
      }
      if (prev == nullptr) {
        throw "first iterator haven't founde";
      }
      Node* current = from.node_;
      while (current != to.node_) {
        Node* tmp = current->next;
        delete current;
        current = tmp;
        size--;
      }
      prev->next = current;
    }
    void clear()
    {
      if (head) {
        clear(begin(), end());
      }
    }
    size_t get_size() {
      return size;
    }
  private:
    Node* head;
    size_t size;
  };
}

int main()
{
  using namespace kitserov;
  List< std::string > names;
  List< List< int > > list_of_lists;
  std::string name;
  while (true) {
    if (!(std::cin >> name)) {
      break;
    }
    names.insert_tail(std::move(name));
    List< int > numbers;
    int num;
    while (std::cin >> num) {
      numbers.insert_tail(std::move(num));
    }
    if (std::cin.eof()) {
      list_of_lists.insert_tail(std::move(numbers));
      break;
    }
    if (std::cin.fail()) {
      std::cin.clear();
      list_of_lists.insert_tail(std::move(numbers));
    }
  }
  std::cout << "\n";
  for (LIter< std::string > it = names.begin(); it != names.end(); ++it) {
    std::cout << *it << " ";
  }
  std::cout << "\n";
  size_t max_size = 0;
  for (LIter< List< int > > it = list_of_lists.begin(); it != list_of_lists.end(); ++it) {
    size_t s = it->get_size();
    if (s > max_size) {
      max_size = s;
    }
  }
  for (size_t i = 0; i < max_size; i++) {
    for (size_t j = 0; j < list_of_lists.get_size(); j++) {
      List< int >& curList = *(list_of_lists[j]);
      if (curList.get_size() > i) {
        std::cout << *(curList[i]) << " ";
      }
    }
    std::cout << "\n";
  }

  names.clear();
  for (LIter< List< int > > it = list_of_lists.begin(); it != list_of_lists.end(); ++it) {
    (*it).clear();
  }
  list_of_lists.clear();

  return 0;
}
