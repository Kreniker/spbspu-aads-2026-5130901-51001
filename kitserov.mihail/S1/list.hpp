#ifndef LIST_HPP
#define LIST_HPP

#include <cstddef>
#include <stdexcept>

namespace kitserov
{
  template< class T > class List;

  template < class T >
  class LIter
  {
    friend class List< T >;
    typename List< T >::Node* node_;

  public:
    LIter() :
      node_(nullptr)
    {}

    LIter(typename List< T >::Node* n) :
      node_(n)
    {}

    T& operator*() const
    {
      return node_->data_;
    }

    T* operator->() const
    {
      return &node_->data_;
    }

    LIter& operator++()
    {
      if (node_) {
        node_ = node_->next_;
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
    typename List< T >::Node* node_;

  public:
    LCIter() :
      node_(nullptr)
    {}

    explicit LCIter(typename List< T >::Node* n) :
      node_(n)
    {}

    LCIter(const LIter< T >& it) :
      node_(it.node_)
    {}

    const T& operator*() const
    {
      return node_->data_;
    }

    const T* operator->() const
    {
      return &node_->data_;
    }

    LCIter& operator++()
    {
      if (node_) {
        node_ = node_->next_;
      }
      return *this;
    }

    bool operator==(const LCIter& other) const
    {
      return node_ == other.node_;
    }

    bool operator!=(const LCIter& other) const
    {
      return node_ != other.node_;
    }
  };

  template< class T >
  struct List
  {
  public:
    struct Node
    {
      T data_;
      Node* next_;

      Node(T& v, Node* n = nullptr) :
        data_(std::move(v)),
        next_(n)
      {}
    };

    List() noexcept :
      head_(nullptr),
      size_(0)
    {}

    ~List() noexcept
    {
      clear();
    }

    List(List& other)
    {
      for (LIter< T > it = other.begin(); it != other.end(); ++it) {
        insert_tail(T(*it));
      }
    }
    void operator()(List& other)
    {
      (*this).swap(other);
    }
    void swap(List& other) noexcept
    {
      std::swap(head_, other.head_);
      std::swap(size_, other.size_);
    }
    List& operator=(List& other)
    {
      if (this != &other) {
        List tmp(other);
        swap(tmp);
      }
      return *this;
    }

    List(List&& other) noexcept :
      head_(other.head_),
      size_(other.size_)
    {
      other.head_ = nullptr;
      other.size_ = 0;
    }

    List& operator=(List&& other) noexcept
    {
      if (this != &other) {
        clear();
        head_ = other.head_;
        size_ = other.size_;
        other.head_ = nullptr;
        other.size_ = 0;
      }
      return *this;
    }

    LIter< T > begin() noexcept
    {
      return LIter< T >(head_);
    }

    LIter< T > end() noexcept
    {
      return LIter< T >(nullptr);
    }

    LCIter< T > begin() const noexcept
    {
      return LCIter< T >(head_);
    }

    LCIter< T > end() const noexcept
    {
      return LCIter< T >(nullptr);
    }

    LCIter< T > cbegin() const noexcept
    {
      return LCIter< T >(head_);
    }

    LCIter< T > cend() const noexcept
    {
      return LCIter< T >(nullptr);
    }

    LIter< T > operator[](size_t index)
    {
      if (index >= size_) {
        throw std::out_of_range("out_of_range");
      }
      Node* current = head_;
      for (size_t i = 0; i < index; i++) {
        current = current->next_;
      }
      return LIter< T >(current);
    }

    LCIter< T > operator[](size_t index) const
    {
      if (index >= size_) {
        throw std::out_of_range("out_of_range");
      }
      Node* current = head_;
      for (size_t i = 0; i < index; i++) {
        current = current->next_;
      }
      return LCIter< T >(current);
    }

    LIter< T > add(T& v)
    {
      Node* newNode = new Node(v, head_);
      head_ = newNode;
      size_++;
      return LIter< T >(newNode);
    }

    LIter< T > insert(T& v, LIter< T > pos)
    {
      size_++;
      if (!pos.node_) {
        return add(v);
      }
      Node* newNode = new Node(v, pos.node_->next_);
      pos.node_->next_ = newNode;
      return LIter< T >(newNode);
    }

    LIter< T > insert_tail(T& v)
    {
      if (size_ == 0) {
        return add(v);
      }
      return insert(v, (*this)[size_ - 1]);
    }

    LIter< T > insert_tail(T&& v)
    {
      if (size_ == 0) {
        return add(v);
      }
      return insert(v, (*this)[size_ - 1]);
    }

    T& front()
    {
      if (size_ == 0) {
        throw std::out_of_range("list is empty");
      }
      return head_->data_;
    }

    const T& front() const
    {
      if (size_ == 0) {
        throw std::out_of_range("list is empty");
      }
      return head_->data_;
    }

    T& back()
    {
      if (size_ == 0) {
        throw std::out_of_range("list is empty");
      }
      return *((*this)[size_ - 1]);
    }

    const T& back() const
    {
      if (size_ == 0) {
        throw std::out_of_range("list is empty");
      }
      return *((*this)[size_ - 1]);
    }

    void clear(LIter< T > from, LIter< T > to)
    {
      if (from.node_ == nullptr) {
        return;
      }
      if (head_ == from.node_) {
        Node* newHead = to.node_;
        Node* current = head_;
        while (current != to.node_) {
          Node* tmp = current->next_;
          delete current;
          current = tmp;
          --size_;
        }
        head_ = newHead;
        return;
      }
      Node* prev = head_;
      while (prev != nullptr && prev->next_ != from.node_) {
        prev = prev->next_;
      }
      if (prev == nullptr) {
        throw std::logic_error("first iterator not found");
      }
      Node* current = from.node_;
      while (current != to.node_) {
        Node* tmp = current->next_;
        delete current;
        current = tmp;
        size_--;
      }
      prev->next_ = current;
    }

    void clear()
    {
      if (head_) {
        clear(begin(), end());
      }
    }

    size_t get_size() const noexcept
    {
      return size_;
    }

    LIter< T > pop_front()
    {
      if (!head_) {
        throw std::out_of_range("List is empty");
      }
      Node* oldHead = head_;
      head_ = head_->next_;
      delete oldHead;
      --size_;
      return LIter< T >(head_);
    }

    LIter< T > pop_back()
    {
      if (!head_) {
        throw std::out_of_range("List is empty");
      }
      Node* current;
      if (head_->next_ == nullptr) {
        delete head_;
        head_ = nullptr;
        current = head_;
      } else {
        current = head_;
        while (current->next_->next_ != nullptr) {
          current = current->next_;
        }
        delete current->next_;
        current->next_ = nullptr;
      }
      --size_;
      return LIter< T >(current);
    }

  private:
    Node* head_;
    size_t size_;
  };

  template class List< size_t >;
  template class List< List< size_t > >;
}

#endif
