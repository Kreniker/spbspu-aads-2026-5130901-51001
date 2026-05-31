#ifndef ITEM_HPP
#define ITEM_HPP

#include <ostream>
#include <functional>
#include <stdexcept>
#include <string>

#include "hash-cucushka.hpp"
#include "matrix.hpp"

namespace kitserov
{
  class Item
  {
    std::string id_;
    std::string name_;
    size_t value_;
    size_t width_;
    size_t height_;

  public:
    Item() noexcept :
      id_(),
      name_(),
      value_(0),
      width_(0),
      height_(0)
    {}

    template< class Hash >
    Item(const std::string& id, const std::string& name, size_t value,
         size_t width, size_t height, const Hash& list)
    {
      if (list.contains(id))
      {
        throw std::invalid_argument("Duplicate item id");
      }

      id_ = id;
      name_ = name;
      value_ = value;
      width_ = width;
      height_ = height;
    }

    const std::string& id() const
    {
      return id_;
    }

    const std::string& name() const
    {
      return name_;
    }

    size_t value() const
    {
      return value_;
    }

    size_t width() const
    {
      return width_;
    }

    size_t height() const
    {
      return height_;
    }

    bool operator<(const Item& other) const noexcept
    {
      return value_ < other.value_;
    }

    friend std::ostream& operator<<(std::ostream& out, const Item& item)
    {
      return out << item.name_;
    }
  };

  using ItemCollection = Vector< Item >;
  using Inventory = Matrix< Item >;
  using ItemTable = HashCucushka< std::string, Item,
    std::hash< std::string >, std::hash< std::string >,
    std::equal_to< std::string > >;
  using CollectionTable = HashCucushka< std::string, ItemCollection,
    std::hash< std::string >, std::hash< std::string >,
    std::equal_to< std::string > >;
}

#endif
