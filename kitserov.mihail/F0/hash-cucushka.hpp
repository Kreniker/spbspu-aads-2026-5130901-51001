#ifndef HASHCUCUSHKA_HPP
#define HASHCUCUSHKA_HPP

namespace kitserov
{
  template<class Key, class Value, class Hash1, class Hash2, class Equal>
  class HashCucushka
  {
    static constexpr size_t DEFAULT_CAPACITY = 8;
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
    Slot* slots_;
    template< class Hash >
    size_t hashK(const Key& key) const
    {
      if (capacity_ == 0)
      {
        return 0;
      }
      return Hash(key) % capacity_;
    }
    template< class Hash >
    size_t probe(const Key& key, size_t i) const
    {
      if (capacity_ == 0)
      {
        return 0;
      }
      return (hashK< Hash >(key) + i * i) % capacity_;
    }
  };
}
#endif
