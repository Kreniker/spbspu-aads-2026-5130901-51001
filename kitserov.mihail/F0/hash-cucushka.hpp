#ifndef HASHCUCUSHKA_HPP
#define HASHCUCUSHKA_HPP

#include <cstddef>
#include <functional>
#include <stdexcept>
#include <utility>

namespace kitserov
{
  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  class HashCucushka
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
    Hash1 hash1_;
    Hash2 hash2_;
    size_t size_;
    size_t capacity_;
    Slot* slots1_;
    Slot* slots2_;

    static size_t normalizeCapacity(size_t capacity) noexcept
    {
      if (capacity == 0)
      {
        return 0;
      }
      if (capacity < 2)
      {
        return 2;
      }
      return (capacity % 2 == 0) ? capacity : capacity + 1;
    }

    size_t halfCapacity(size_t tableCapacity) const noexcept
    {
      return tableCapacity / 2;
    }

    size_t firstIndex(const Key& key, size_t tableCapacity) const
    {
      size_t half = halfCapacity(tableCapacity);
      if (half == 0)
      {
        return 0;
      }
      return hash1_(key) % half;
    }

    size_t secondIndex(const Key& key, size_t tableCapacity) const
    {
      size_t half = halfCapacity(tableCapacity);
      if (half == 0)
      {
        return 0;
      }
      return half + (hash2_(key) % half);
    }

    const Slot& slotAt(size_t index) const noexcept
    {
      size_t half = halfCapacity(capacity_);
      return index < half ? slots1_[index] : slots2_[index - half];
    }
    Slot& slotAt(size_t index) noexcept
    {
      size_t half = halfCapacity(capacity_);
      return index < half ? slots1_[index] : slots2_[index - half];
    }

    bool insertPair(Slot* firstTable, Slot* secondTable, size_t tableCapacity,
                    const Key& key, const Value& value, size_t& tableSize) const
    {
      if (tableCapacity == 0)
      {
        return false;
      }

      size_t half = tableCapacity / 2;
      Key currentKey = key;
      Value currentValue = value;
      bool useFirst = true;

      for (size_t step = 0; step < tableCapacity; ++step)
      {
        size_t index = useFirst ? (hash1_(currentKey) % half) : (hash2_(currentKey) % half);
        Slot& slot = useFirst ? firstTable[index] : secondTable[index];

        if (slot.state_ == State::EMPTY || slot.state_ == State::TOMBSTONE)
        {
          slot.key_ = currentKey;
          slot.val_ = currentValue;
          slot.state_ = State::OCCUPIED;
          ++tableSize;
          return true;
        }

        if (equal_(slot.key_, currentKey))
        {
          slot.val_ = currentValue;
          return true;
        }

        std::swap(currentKey, slot.key_);
        std::swap(currentValue, slot.val_);
        slot.state_ = State::OCCUPIED;
        useFirst = !useFirst;
      }

      return false;
    }

    size_t findIndex(const Key& key) const noexcept
    {
      if (capacity_ == 0)
      {
        return capacity_;
      }

      size_t half = halfCapacity(capacity_);
      size_t first = hash1_(key) % half;
      if (slots1_[first].state_ == State::OCCUPIED && equal_(slots1_[first].key_, key))
      {
        return first;
      }

      size_t second = hash2_(key) % half;
      if (slots2_[second].state_ == State::OCCUPIED && equal_(slots2_[second].key_, key))
      {
        return half + second;
      }

      return capacity_;
    }

    void destroy() noexcept
    {
      delete[] slots1_;
      delete[] slots2_;
      slots1_ = nullptr;
      slots2_ = nullptr;
      size_ = 0;
      capacity_ = 0;
    }

  public:
    HashCucushka() :
      size_(0),
      capacity_(DEFAULT_CAPACITY),
      slots1_(new Slot[DEFAULT_CAPACITY / 2]),
      slots2_(new Slot[DEFAULT_CAPACITY / 2])
    {}

    explicit HashCucushka(size_t size) :
      size_(0),
      capacity_(normalizeCapacity(size)),
      slots1_(capacity_ == 0 ? nullptr : new Slot[capacity_ / 2]),
      slots2_(capacity_ == 0 ? nullptr : new Slot[capacity_ / 2])
    {}

    HashCucushka(const HashCucushka& other) :
      equal_(other.equal_),
      hash1_(other.hash1_),
      hash2_(other.hash2_),
      size_(other.size_),
      capacity_(other.capacity_),
      slots1_(capacity_ == 0 ? nullptr : new Slot[capacity_ / 2]),
      slots2_(capacity_ == 0 ? nullptr : new Slot[capacity_ / 2])
    {
      size_t half = halfCapacity(capacity_);
      for (size_t i = 0; i < half; ++i)
      {
        slots1_[i] = other.slots1_[i];
        slots2_[i] = other.slots2_[i];
      }
    }

    HashCucushka(HashCucushka&& other) noexcept :
      equal_(std::move(other.equal_)),
      hash1_(std::move(other.hash1_)),
      hash2_(std::move(other.hash2_)),
      size_(other.size_),
      capacity_(other.capacity_),
      slots1_(other.slots1_),
      slots2_(other.slots2_)
    {
      other.size_ = 0;
      other.capacity_ = 0;
      other.slots1_ = nullptr;
      other.slots2_ = nullptr;
    }

    ~HashCucushka()
    {
      destroy();
    }

    HashCucushka& operator=(const HashCucushka& other)
    {
      if (this == &other)
      {
        return *this;
      }

      HashCucushka tmp(other);
      swap(tmp);
      return *this;
    }

    HashCucushka& operator=(HashCucushka&& other) noexcept
    {
      if (this == &other)
      {
        return *this;
      }

      swap(other);
      return *this;
    }

    bool isEmpty() const noexcept
    {
      return size_ == 0;
    }

    void copy(const HashCucushka& other)
    {
      if (this == &other)
      {
        return;
      }
      *this = other;
    }

    void swap(HashCucushka& other) noexcept
    {
      using std::swap;
      swap(equal_, other.equal_);
      swap(hash1_, other.hash1_);
      swap(hash2_, other.hash2_);
      swap(size_, other.size_);
      swap(capacity_, other.capacity_);
      swap(slots1_, other.slots1_);
      swap(slots2_, other.slots2_);
    }

    Value* find(const Key& key) noexcept
    {
      size_t index = findIndex(key);
      if (index == capacity_)
      {
        return nullptr;
      }
      size_t half = halfCapacity(capacity_);
      return index < half ? &slots1_[index].val_ : &slots2_[index - half].val_;
    }

    const Value* find(const Key& key) const noexcept
    {
      size_t index = findIndex(key);
      if (index == capacity_)
      {
        return nullptr;
      }
      size_t half = halfCapacity(capacity_);
      return index < half ? &slots1_[index].val_ : &slots2_[index - half].val_;
    }

    bool contains(const Key& key) const
    {
      return find(key) != nullptr;
    }

    bool add(const Key& key, const Value& value)
    {
      Value* existing = find(key);
      if (existing != nullptr)
      {
        *existing = value;
        return true;
      }

      if (capacity_ == 0)
      {
        rehash(DEFAULT_CAPACITY);
      }

      while (true)
      {
        Slot* candidate1 = new Slot[capacity_ / 2];
        Slot* candidate2 = new Slot[capacity_ / 2];
        size_t half = capacity_ / 2;
        for (size_t i = 0; i < half; ++i)
        {
          candidate1[i] = slots1_[i];
          candidate2[i] = slots2_[i];
        }

        size_t candidateSize = size_;
        if (insertPair(candidate1, candidate2, capacity_, key, value, candidateSize))
        {
          delete[] slots1_;
          delete[] slots2_;
          slots1_ = candidate1;
          slots2_ = candidate2;
          size_ = candidateSize;
          return true;
        }

        delete[] candidate1;
        delete[] candidate2;
        rehash(capacity_ == 0 ? DEFAULT_CAPACITY : capacity_ * 2 + 1);
      }
    }

    Value& operator[](const Key& key)
    {
      Value* ptr = find(key);
      if (ptr == nullptr)
      {
        throw std::out_of_range("Key not found");
      }
      return *ptr;
    }

    const Value& operator[](const Key& key) const
    {
      const Value* ptr = find(key);
      if (ptr == nullptr)
      {
        throw std::out_of_range("Key not found");
      }
      return *ptr;
    }

    bool erase(const Key& key)
    {
      size_t index = findIndex(key);
      if (index == capacity_)
      {
        return false;
      }

      size_t half = halfCapacity(capacity_);
      if (index < half)
      {
        slots1_[index].state_ = State::TOMBSTONE;
      }
      else
      {
        slots2_[index - half].state_ = State::TOMBSTONE;
      }
      --size_;
      return true;
    }

    float loadFactor() const noexcept
    {
      if (capacity_ == 0)
      {
        return 1.0f;
      }
      return static_cast< float >(size_) / static_cast< float >(capacity_);
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
      newCapacity = normalizeCapacity(newCapacity);
      if (newCapacity == 0)
      {
        destroy();
        return;
      }

      Slot* oldSlots1 = slots1_;
      Slot* oldSlots2 = slots2_;
      size_t oldCapacity = capacity_;
      size_t targetCapacity = newCapacity;

      while (true)
      {
        Slot* newSlots1 = new Slot[targetCapacity / 2];
        Slot* newSlots2 = new Slot[targetCapacity / 2];
        size_t newSize = 0;
        bool ok = true;
        size_t half = oldCapacity / 2;

        for (size_t i = 0; i < half; ++i)
        {
          if (oldSlots1[i].state_ == State::OCCUPIED)
          {
            if (!insertPair(newSlots1, newSlots2, targetCapacity, oldSlots1[i].key_, oldSlots1[i].val_, newSize))
            {
              ok = false;
              break;
            }
          }

          if (ok && oldSlots2[i].state_ == State::OCCUPIED)
          {
            if (!insertPair(newSlots1, newSlots2, targetCapacity, oldSlots2[i].key_, oldSlots2[i].val_, newSize))
            {
              ok = false;
              break;
            }
          }
        }

        if (ok)
        {
          delete[] slots1_;
          delete[] slots2_;
          slots1_ = newSlots1;
          slots2_ = newSlots2;
          size_ = newSize;
          capacity_ = targetCapacity;
          return;
        }

        delete[] newSlots1;
        delete[] newSlots2;
        targetCapacity = normalizeCapacity(targetCapacity * 2 + 1);
      }
    }

    class iterator
    {
      friend class HashCucushka;
      HashCucushka* table_ = nullptr;
      size_t idx_ = 0;

      iterator(HashCucushka* table, size_t idx) noexcept :
        table_(table),
        idx_(idx)
      {}

    public:
      iterator() noexcept = default;

      Value& operator*() const
      {
        return table_ -> slotAt(idx_).val_;
      }

      Value* operator -> () const
      {
        return &(table_ -> slotAt(idx_).val_);
      }

      const Key& key() const noexcept
      {
        return table_ -> slotAt(idx_).key_;
      }

      iterator& operator++() noexcept
      {
        if (table_ == nullptr || idx_ >= table_ -> capacity_)
        {
          return *this;
        }

        ++idx_;
        while (idx_ < table_ -> capacity_ && table_ -> slotAt(idx_).state_ != State::OCCUPIED)
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

      bool operator==(const iterator& other) const noexcept
      {
        return table_ == other.table_ && idx_ == other.idx_;
      }

      bool operator!=(const iterator& other) const noexcept
      {
        return !(*this == other);
      }

      bool operator<(const iterator& other) const noexcept
      {
        return table_ == other.table_ && idx_ < other.idx_;
      }

      bool operator>(const iterator& other) const noexcept
      {
        return table_ == other.table_ && idx_ > other.idx_;
      }
    };

    class const_iterator
    {
      friend class HashCucushka;
      const HashCucushka* table_ = nullptr;
      size_t idx_ = 0;

      const_iterator(const HashCucushka* table, size_t idx) noexcept :
        table_(table),
        idx_(idx)
      {}

    public:
      const_iterator() noexcept = default;

      const_iterator(const iterator& other) noexcept :
        table_(other.table_),
        idx_(other.idx_)
      {}

      const Value& operator*() const
      {
        return table_ -> slotAt(idx_).val_;
      }

      const Value* operator -> () const
      {
        return &(table_ -> slotAt(idx_).val_);
      }

      const Key& key() const noexcept
      {
        return table_ -> slotAt(idx_).key_;
      }

      const_iterator& operator++() noexcept
      {
        if (table_ == nullptr || idx_ >= table_ -> capacity_)
        {
          return *this;
        }

        ++idx_;
        while (idx_ < table_ -> capacity_ && table_ -> slotAt(idx_).state_ != State::OCCUPIED)
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

      bool operator==(const const_iterator& other) const noexcept
      {
        return table_ == other.table_ && idx_ == other.idx_;
      }

      bool operator!=(const const_iterator& other) const noexcept
      {
        return !(*this == other);
      }

      bool operator<(const const_iterator& other) const noexcept
      {
        return table_ == other.table_ && idx_ < other.idx_;
      }

      bool operator>(const const_iterator& other) const noexcept
      {
        return table_ == other.table_ && idx_ > other.idx_;
      }
    };

    iterator begin() noexcept
    {
      for (size_t i = 0; i < capacity_; ++i)
      {
        if (slotAt(i).state_ == State::OCCUPIED)
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
        if (slotAt(i).state_ == State::OCCUPIED)
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
}

#endif
