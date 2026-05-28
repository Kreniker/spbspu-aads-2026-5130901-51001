#ifndef BITREE_HPP
#define BITREE_HPP

#include <cstddef>
#include <stdexcept>
#include <utility>
#include <algorithm>

namespace kitserov
{
  template < class Key, class Value, class Compare >
  class BSTIterator;
  template < class Key, class Value, class Compare >
  class BSTConstIterator;
  template < class Key, class Value, class Compare = std::less< Key > >
  class BSTree
  {
    std::pair< const Key, Value > data_;
    BSTree* left_;
    BSTree* right_;
    BSTree* parent_;
    size_t height_;
    Compare cmp_;
    size_t size_;
    bool is_fake_root_;

    BSTree(const Key& k, const Value& v, BSTree* p, const Compare& comp)
        : data_(k, v), left_(nullptr), right_(nullptr), parent_(p), height_(1),
        cmp_(comp), size_(1), is_fake_root_(false) {}

    BSTree(const Compare& comp)
        : data_(), left_(nullptr), right_(nullptr), parent_(nullptr), height_(0),
        cmp_(comp), size_(0), is_fake_root_(true) {}

    static BSTree* clone(const BSTree* other, BSTree* parent)
    {
      if (!other) {
        return nullptr;
      }
      BSTree* node = new BSTree(other -> data_.first, other -> data_.second, parent,
        other -> cmp_);
      node -> height_ = other -> height_;
      node -> size_ = other -> size_;
      node -> left_ = clone(other -> left_, node);
      node -> right_ = clone(other -> right_, node);
      return node;
    }

    void update() noexcept
    {
      if (is_fake_root_) {
        if (left_) {
          height_ = left_ -> height_;
          size_ = left_ -> size_;
        } else {
          height_ = 0;
          size_ = 0;
        }
      } else {
        if (left_ && right_) {
          height_ = 1 + std::max(left_ -> height_, right_ -> height_);
          size_ = 1 + (left_ -> size_ + right_ -> size_);
        } else if (left_) {
          height_ = 1 + left_ -> height_;
          size_ = 1 + left_ -> size_;
        } else if (right_) {
          height_ = 1 + right_ -> height_;
          size_ = 1 + right_ -> size_;
        } else {
          height_ = 1;
          size_ = 1;
        }
      }
    }

    BSTree* rotateLeft(BSTree* child)
    {
      BSTree* parent = child -> parent_;
      BSTree* grand = parent -> parent_;
      parent -> right_ = child -> left_;
      if (child -> left_) {
        child -> left_ -> parent_ = parent;
      }
      child -> left_ = parent;
      parent -> parent_ = child;
      child -> parent_ = grand;
      if (grand) {
        if (grand -> left_ == parent) {
          grand -> left_ = child;
        } else {
          grand -> right_ = child;
        }
      }
      parent -> update();
      child -> update();
      if (grand) {
        grand -> update();
      }
      return child;
    }

    BSTree* rotateRight(BSTree* child)
    {
      BSTree* parent = child -> parent_;
      BSTree* grand = parent -> parent_;
      parent -> left_ = child -> right_;
      if (child -> right_) {
        child -> right_ -> parent_ = parent;
      }
      child -> right_ = parent;
      parent -> parent_ = child;
      child -> parent_ = grand;
      if (grand) {
        if (grand -> left_ == parent) {
          grand -> left_ = child;
        } else {
          grand -> right_ = child;
        }
      }
      parent -> update();
      child -> update();
      if (grand) {
        grand -> update();
      }
      return child;
    }

    BSTree* find_root(const Key& k)
    {
      BSTree* cur = left_;
      while (cur) {
        if (cmp_(k, cur -> data_.first)) {
          cur = cur -> left_;
        } else if (cmp_(cur -> data_.first, k)) {
          cur = cur -> right_;
        } else {
          return cur;
        }
      }
      return nullptr;
    }

    const BSTree* find_root(const Key& k) const
    {
      return const_cast< BSTree* >(this) -> find_root(k);
    }

    static void destroy_subtree(BSTree* node)
    {
      if (!node) {
        return;
      }
      destroy_subtree(node -> left_);
      destroy_subtree(node -> right_);
      node -> left_ = nullptr;
      node -> right_ = nullptr;
      delete node;
    }

    void clear()
    {
      destroy_subtree(left_);
      destroy_subtree(right_);
      left_ = nullptr;
      right_ = nullptr;
      parent_ = nullptr;
      height_ = 0;
      size_ = 0;
    }

  public:
    using iterator = BSTIterator< Key, Value, Compare> ;
    using const_iterator = BSTConstIterator< Key, Value, Compare >;
    friend class BSTIterator< Key, Value, Compare >;
    friend class BSTConstIterator< Key, Value, Compare >;

    BSTree() : data_(), left_(nullptr), right_(nullptr), parent_(nullptr), height_(0),
      cmp_(Compare()), size_(0), is_fake_root_(true) {}
    ~BSTree()
    {
      clear();
    }
    BSTree(const BSTree& other) : BSTree()
    {
      copy(other);
    }
    BSTree(BSTree&& other) noexcept : BSTree()
    {
      swap(other);
    }
    BSTree& operator=(const BSTree& other)
    {
      copy(other);
      return *this;
    }
    BSTree& operator=(BSTree&& other)
    {
      swap(other);
      return *this;
    }

    void swap(BSTree&& other) noexcept
    {
      std::swap(data_, other.data_);
      std::swap(left_, other.left_);
      std::swap(right_, other.right_);
      std::swap(parent_, other.parent_);
      std::swap(height_, other.height_);
      std::swap(size_, other.size_);
      std::swap(cmp_, other.cmp_);
      if (left_) {
        left_ -> parent_ = this;
      }
      if (right_) {
        right_ -> parent_ = this;
      }
    }
    void copy(const BSTree& other)
    {
      if (this == &other) {
        return;
      }
      clear();
      left_ = clone(other.left_, this);
      right_ = clone(other.right_, this);
      height_ = other.height_;
      size_ = other.size_;
      cmp_ = other.cmp_;
    }
    size_t height() const noexcept
    {
      return height_;
    }

    size_t height(const_iterator it) const
    {
      return it.node -> height();
    }

    size_t size() const noexcept
    {
      return size_;
    }
    bool empty() const
    {
      return size_ == 0;
    }
    void push(const Key& k, const Value& v)
    {
      if (!left_) {
        left_ = new BSTree(k, v, this, cmp_);
        update();
        return;
      }

      BSTree* cur = left_;
      BSTree* parent = nullptr;
      bool left_child = false;

      while (cur) {
        parent = cur;
        if (cmp_(k, cur -> data_.first)) {
          cur = cur -> left_;
          left_child = true;
        } else if (cmp_(cur -> data_.first, k)) {
          cur = cur -> right_;
          left_child = false;
        } else {
          const_cast< Value& >(cur -> data_.second) = v;
          return;
        }
      }

      BSTree* newNode = new BSTree(k, v, parent, cmp_);
      if (left_child) {
        parent -> left_ = newNode;
      } else {
        parent -> right_ = newNode;
      }

      while (parent) {
        parent -> update();
        parent = parent -> parent_;
      }
    }
    Value& get(const Key& k)
    {
      BSTree* n = find_root(k);
      if (!n) {
        throw std::out_of_range("BSTree::get: key not found");
      }
      return n -> data_.second;
    }
    const Value& get(const Key& k) const
    {
        return const_cast< BSTree* >(this) -> get(k);
    }
    bool contains(const Key& k) const
    {
      return find_root(k) != nullptr;
    }
    Value drop(const Key& k)
    {
      BSTree* n = find_root(k);
      if (!n) {
        throw std::out_of_range("BSTree::drop: key not found");
      }
      Value removed = n -> data_.second;
      BSTree* child = nullptr;
      BSTree* parent = n -> parent_;
      if (!n -> left_ && !n -> right_) {
        if (parent -> left_ == n) {
          parent -> left_ = nullptr;
        } else {
          parent -> right_ = nullptr;
        }
      } else if (!n -> left_ || !n -> right_) {
        child = n -> left_ ? n -> left_ : n -> right_;
        child -> parent_ = parent;
        if (parent -> left_ == n) {
          parent -> left_ = child;
        } else {
          parent -> right_ = child;
        }
      } else {
        BSTree* succ = n -> right_;
        while (succ -> left_) {
          succ = succ -> left_;
        }
        if (succ -> parent_ != n) {
          if (succ -> right_) {
            succ -> right_ -> parent_ = succ -> parent_;
            succ -> parent_ -> left_ = succ -> right_;
          } else {
            succ -> parent_ -> left_ = nullptr;
          }
          succ -> right_ = n -> right_;
          if (n -> right_) {
            n -> right_ -> parent_ = succ;
          }
        }

        succ -> left_ = n -> left_;
        if (n -> left_) {
          n -> left_ -> parent_ = succ;
        }
        succ -> parent_ = n -> parent_;
        if (n -> parent_ -> left_ == n) {
          n -> parent_ -> left_ = succ;
        } else {
          n -> parent_ -> right_ = succ;
        }
        child = succ;
        parent = succ -> parent_;
      }

      BSTree* update_node = parent;
      while (update_node) {
        update_node -> update();
        update_node = update_node -> parent_;
      }
      delete n;
      return removed;
    }
    iterator begin()
    {
      if (size() == 0) {
        return end();
      }
      BSTree* n = left_;
      while (n -> left_) {
        n = n -> left_;
      }
      return iterator(n, this);
    }
    iterator end()
    {
      return iterator(this, this);
    }
    const_iterator begin() const
    {
      return cbegin();
    }
    const_iterator end() const
    {
      return cend();
    }
    const_iterator cbegin() const
    {
      if (size() == 0) {
        return cend();
      }
      const BSTree* n = left_;
      while (n -> left_) {
        n = n -> left_;
      }
      return const_iterator(n, this);
    }
    const_iterator cend() const
    {
      return const_iterator(this, this);
    }
    iterator find(const Key& k)
    {
      BSTree* n = find_root(k);
      return n ? iterator(n, this) : end();
    }

    const_iterator find(const Key& k) const
    {
      const BSTree* n = find_root(k);
      return n ? const_iterator(n, this) : cend();
    }
    const_iterator rotateLeft(const_iterator it)
    {
      if (!it.node || it.node == this || it.node -> is_fake_root_ || !it.node -> parent_ ||
          it.node -> parent_ -> is_fake_root_ || it.node != it.node -> parent_ -> right_) {
        throw std::invalid_argument("rotateLeft: bad argument");
      }
      BSTree* child = const_cast< BSTree* >(it.node);
      BSTree* new_subroot = rotateLeft(child);
      return const_iterator(new_subroot, this);
    }
    const_iterator rotateRight(const_iterator it)
    {
      if (!it.node || it.node == this || it.node -> is_fake_root_ || !it.node -> parent_
          || it.node -> parent_ -> is_fake_root_ || it.node != it.node -> parent_ -> left_) {
        throw std::invalid_argument("rotateRight: bad argument");
      }
      BSTree* child = const_cast< BSTree* >(it.node);
      BSTree* new_subroot = rotateRight(child);
      return const_iterator(new_subroot, this);
    }
    const_iterator rotateLargeLeft(const_iterator it)
    {
      if (!it.node || it.node == this || it.node -> is_fake_root_ || !it.node -> parent_
          || !it.node -> parent_ -> parent_ ||
          it.node != it.node -> parent_ -> left_
          || it.node -> parent_ != it.node -> parent_ -> parent_ -> right_) {
        throw std::invalid_argument("rotateLargeLeft: bad argument");
      }
      rotateRight(it);
      return rotateLeft(it);
    }
    const_iterator rotateLargeRight(const_iterator it)
    {
      if (!it.node || it.node == this || it.node -> is_fake_root_ || !it.node -> parent_
          || !it.node -> parent_ -> parent_ ||
          it.node != it.node -> parent_ -> right_
          || it.node -> parent_ != it.node -> parent_ -> parent_ -> left_) {
        throw std::invalid_argument("rotateLargeRight: bad argument");
      }
      rotateLeft(it);
      return rotateRight(it);
    }
  };
  template < class Key, class Value, class Compare >
  class BSTIterator
  {
    using Node = BSTree< Key, Value, Compare >;
    Node* node_;
    Node* sentinel_;

    explicit BSTIterator(Node* n, Node* s) : node_(n), sentinel_(s) {}
    friend class BSTConstIterator< Key, Value, Compare >;
    friend class BSTree< Key, Value, Compare >;

  public:
    using value_type = std::pair< const Key, Value >;

    BSTIterator() : node_(nullptr), sentinel_(nullptr) {}

    value_type& operator*() const
    {
      return node_ -> data_;
    }

    value_type* operator -> () const
    {
      return &(node_ -> data_);
    }

    BSTIterator& operator++()
    {
      if (node_ -> right_) {
        node_ = node_ -> right_;
        while (node_ -> left_) {
          node_ = node_ -> left_;
        }
      } else {
        Node* p = node_ -> parent_;
        while (p != sentinel_ && node_ == p -> right_) {
          node_ = p;
          p = p -> parent_;
        }
        node_ = p;
      }
      return *this;
    }

    BSTIterator operator++(int)
    {
      BSTIterator tmp(*this);
      ++(*this);
      return tmp;
    }

    bool operator==(const BSTIterator& other) const
    {
      return node_ == other.node_;
    }

    bool operator!=(const BSTIterator& other) const
    {
      return node_ != other.node_;
    }
  };
  template < class Key, class Value, class Compare >
  class BSTConstIterator
  {
    using Node = BSTree< Key, Value, Compare >;
    const Node* node_;
    const Node* sentinel_;

    explicit BSTConstIterator(const Node* n, const Node* s) : node_(n), sentinel_(s) {}
    friend class BSTree< Key, Value, Compare >;

  public:
    using value_type = const std::pair< const Key, Value>;

    BSTConstIterator() : node_(nullptr), sentinel_(nullptr) {}

    BSTConstIterator(const BSTIterator< Key, Value, Compare >& it)
      : node_(it.node_), sentinel_(it.sentinel_) {}

    value_type& operator*() const
    {
      return node_ -> data_;
    }

    value_type* operator -> () const
    {
      return &(node_ -> data_);
    }

    BSTConstIterator& operator++()
    {
      if (node_ -> right_) {
        node_ = node_ -> right_;
        while (node_ -> left_) {
          node_ = node_ -> left_;
        }
      } else {
        const Node* p = node_ -> parent_;
        while (p != sentinel_ && node_ == p -> right_) {
          node_ = p;
          p = p -> parent_;
        }
        node_ = p;
      }
      return *this;
    }

    BSTConstIterator operator++(int)
    {
      BSTConstIterator tmp(*this);
      ++(*this);
      return tmp;
    }

    bool operator==(const BSTConstIterator& other) const
    {
      return node_ == other.node_;
    }

    bool operator!=(const BSTConstIterator& other) const
    {
      return node_ != other.node_;
    }
  };
}

#endif
