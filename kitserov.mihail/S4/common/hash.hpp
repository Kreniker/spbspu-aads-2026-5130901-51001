#ifndef HASH_HPP
#define HASH_HPP

#include <vector>
#include <utility>
#include <cstddef>
#include <stdexcept>
#include <boost/hash2/hash_append.hpp>
#include <boost/hash2/siphash.hpp>

namespace kitserov
{

template<class Key, class Value, class Hash, class Equal>
class HashTable
{
public:
  static constexpr size_t DEFAULT_CAPACITY = 8;

private:
  enum State
  {
    EMPTY = 0,
    OCCUPIED = 1,
    TOMBSTONE = 2
  };

  struct Slot
  {
    Key key_;
    Value val_;
    State state_ = State::EMPTY;
  };

  Equal equal_;
  Hash hash_;
  size_t size_;
  size_t capacity_;
  std::vector<Slot> slots_;

  size_t hashK(const Key& key) const
  {
    if (capacity_ == 0)
    {
      return 0;
    }
    return hash_(key) % capacity_;
  }

  size_t probe(const Key& key, size_t i) const
  {
    if (capacity_ == 0)
    {
      return 0;
    }
    return (hashK(key) + i * i) % capacity_;
  }

public:
  HashTable() :
    size_(0),
    capacity_(DEFAULT_CAPACITY),
    slots_(DEFAULT_CAPACITY)
  {}

  explicit HashTable(size_t size) :
    size_(0),
    capacity_(size),
    slots_(capacity_)
  {}

  HashTable(const HashTable& other)
  {
    copy(other);
  }

  HashTable(HashTable&& other) noexcept
  {
    swap(other);
  }

  HashTable& operator=(const HashTable& other)
  {
    copy(other);
    return *this;
  }

  HashTable& operator=(HashTable&& other) noexcept
  {
    swap(other);
    return *this;
  }

  bool isEmpty() const noexcept
  {
    return size_ == 0;
  }

  void copy(const HashTable& other)
  {
    if (this == &other)
    {
      return;
    }
    HashTable tmp(other.capacity_);
    tmp.slots_ = other.slots_;
    tmp.size_ = other.size_;
    swap(tmp);
  }

  void swap(HashTable& other) noexcept
  {
    std::swap(size_, other.size_);
    std::swap(capacity_, other.capacity_);
    std::swap(slots_, other.slots_);
  }

  Value* find(const Key& key) noexcept
  {
    for (size_t i = 0; i < capacity_; ++i)
    {
      size_t idx = probe(key, i);
      if (slots_[idx].state_ == State::EMPTY)
      {
        return nullptr;
      }
      if (equal_(slots_[idx].key_, key) && slots_[idx].state_ == State::OCCUPIED)
      {
        return &slots_[idx].val_;
      }
    }
    return nullptr;
  }

  const Value* find(const Key& key) const noexcept
  {
    for (size_t i = 0; i < capacity_; ++i)
    {
      size_t idx = probe(key, i);
      if (slots_[idx].state_ == State::EMPTY)
      {
        return nullptr;
      }
      if (equal_(slots_[idx].key_, key) && slots_[idx].state_ == State::OCCUPIED)
      {
        return &slots_[idx].val_;
      }
    }
    return nullptr;
  }

  bool contains(const Key& key) const
  {
    return find(key) != nullptr;
  }

  bool add(const Key& key, const Value& value)
  {
    if (loadFactor() >= 0.8f)
    {
      throw std::overflow_error("Load factor too high");
    }
    for (size_t i = 0; i < capacity_; ++i)
    {
      size_t idx = probe(key, i);
      if (slots_[idx].state_ == State::EMPTY)
      {
        slots_[idx].key_ = key;
        slots_[idx].val_ = value;
        slots_[idx].state_ = State::OCCUPIED;
        ++size_;
        return true;
      }
      if (equal_(slots_[idx].key_, key))
      {
        slots_[idx].val_ = value;
        return true;
      }
    }
    return false;
  }

  Value& operator[](const Key& key)
  {
    Value* ptr = find(key);
    if (!ptr)
    {
      throw std::out_of_range("Key not found");
    }
    return *ptr;
  }

  const Value& operator[](const Key& key) const
  {
    const Value* ptr = find(key);
    if (!ptr)
    {
      throw std::out_of_range("Key not found");
    }
    return *ptr;
  }

  bool erase(const Key& key)
  {
    for (size_t i = 0; i < capacity_; ++i)
    {
      size_t idx = probe(key, i);
      if (slots_[idx].state_ == State::EMPTY)
      {
        return false;
      }
      if (equal_(slots_[idx].key_, key))
      {
        slots_[idx].state_ = State::TOMBSTONE;
        --size_;
        return true;
      }
    }
    return false;
  }

  float loadFactor() const noexcept
  {
    if (capacity_ == 0)
    {
      return 1.0f;
    }
    return static_cast<float>(size_) / static_cast<float>(capacity_);
  }

  size_t size() const noexcept
  {
    return size_;
  }

  size_t capacity() const noexcept
  {
    return capacity_;
  }

  void rehash(size_t newCapacity)
  {
    std::vector<Slot> newSlots(newCapacity);
    size_t oldCapacity = capacity_;
    capacity_ = newCapacity;
    for (size_t i = 0; i < oldCapacity; ++i)
    {
      const Slot& slot = slots_[i];
      try
      {
        if (slot.state_ == State::OCCUPIED)
        {
          size_t probeI = 0;
          size_t idx = probe(slot.key_, probeI);
          while (newSlots[idx].state_ != State::EMPTY)
          {
            ++probeI;
            idx = probe(slot.key_, probeI);
          }
          newSlots[idx] = slot;
        }
      }
      catch (...)
      {
        capacity_ = oldCapacity;
        throw;
      }
    }
    slots_ = std::move(newSlots);
  }

  class iterator
  {
    friend class HashTable;
    HashTable* table_ = nullptr;
    size_t idx_;

    iterator(HashTable* t, size_t idx) noexcept :
      table_(t),
      idx_(idx)
    {}

  public:
    iterator() noexcept = default;

    Value& operator*() const
    {
      return table_->slots_[idx_].val_;
    }

    Value* operator->() const
    {
      return &(table_->slots_[idx_].val_);
    }

    const Key& key() const noexcept
    {
      return table_->slots_[idx_].key_;
    }

    iterator& operator++() noexcept
    {
      size_t cap = table_->capacity();
      if (idx_ >= cap)
      {
        return *this;
      }
      ++idx_;
      while (idx_ < cap && table_->slots_[idx_].state_ != State::OCCUPIED)
      {
        ++idx_;
      }
      return *this;
    }

    iterator operator++(int) noexcept
    {
      iterator tmp = *this;
      ++(*this);
      return tmp;
    }

    bool operator==(const iterator& o) const noexcept
    {
      return table_ == o.table_ && idx_ == o.idx_;
    }

    bool operator!=(const iterator& o) const noexcept
    {
      return !(*this == o);
    }

    bool operator<(const iterator& o) const noexcept
    {
      return table_ == o.table_ && idx_ < o.idx_;
    }

    bool operator>(const iterator& o) const noexcept
    {
      return table_ == o.table_ && idx_ > o.idx_;
    }
  };

  class const_iterator
  {
    friend class HashTable;
    const HashTable* table_ = nullptr;
    size_t idx_;

    const_iterator(const HashTable* t, size_t idx) noexcept :
      table_(t),
      idx_(idx)
    {}

  public:
    const_iterator() noexcept = default;

    const_iterator(const iterator& it) noexcept :
      table_(it.table_),
      idx_(it.idx_)
    {}

    const Value& operator*() const
    {
      return table_->slots_[idx_].val_;
    }

    const Value* operator->() const
    {
      return &(table_->slots_[idx_].val_);
    }

    const Key& key() const noexcept
    {
      return table_->slots_[idx_].key_;
    }

    const_iterator& operator++() noexcept
    {
      size_t cap = table_->capacity();
      if (idx_ >= cap)
      {
        return *this;
      }
      ++idx_;
      while (idx_ < cap && table_->slots_[idx_].state_ != State::OCCUPIED)
      {
        ++idx_;
      }
      return *this;
    }

    const_iterator operator++(int) noexcept
    {
      const_iterator tmp = *this;
      ++(*this);
      return tmp;
    }

    bool operator==(const const_iterator& o) const noexcept
    {
      return table_ == o.table_ && idx_ == o.idx_;
    }

    bool operator!=(const const_iterator& o) const noexcept
    {
      return !(*this == o);
    }

    bool operator<(const const_iterator& o) const noexcept
    {
      return table_ == o.table_ && idx_ < o.idx_;
    }

    bool operator>(const const_iterator& o) const noexcept
    {
      return table_ == o.table_ && idx_ > o.idx_;
    }
  };

  iterator begin() noexcept
  {
    for (size_t i = 0; i < capacity_; ++i)
    {
      if (slots_[i].state_ == State::OCCUPIED)
      {
        return iterator(this, i);
      }
    }
    return end();
  }

  iterator end() noexcept
  {
    return iterator(this, capacity_);
  }

  const_iterator begin() const noexcept
  {
    for (size_t i = 0; i < capacity_; ++i)
    {
      if (slots_[i].state_ == State::OCCUPIED)
      {
        return const_iterator(this, i);
      }
    }
    return end();
  }

  const_iterator end() const noexcept
  {
    return const_iterator(this, capacity_);
  }

  const_iterator cbegin() const noexcept
  {
    return begin();
  }

  const_iterator cend() const noexcept
  {
    return end();
  }
};

template<class T>
struct SipHash
{
  size_t operator()(const T& key) const
  {
    boost::hash2::siphash_64 hasher;
    boost::hash2::hash_append(hasher, {}, key);
    return hasher.result();
  }
};

template<class T1, class T2, template<class> class Hash>
struct PairHash
{
  size_t operator()(const std::pair<T1, T2>& p) const
  {
    Hash<T1> h1;
    Hash<T2> h2;
    return h1(p.first) ^ (h2(p.second) << 1);
  }
};

}

#endif
