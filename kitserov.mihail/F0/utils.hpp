#ifndef UTILS_HPP
#define UTILS_HPP

#include "item.hpp"
#include <algorithm>
#include <istream>
#include <ostream>
#include <vector>

using namespace kitserov;
inline void define_item(std::ostream& out, std::istream& in, ItemTable& items, CollectionTable& collections, Inventory& inventory)
{
  std::string id, name;
  size_t value, width, height;
  (void) collections;
  (void) inventory;

  in >> id >> name >> width >> height >> value;

  const Item* existing = items.find(id);
  if (existing != nullptr)
  {
    throw std::invalid_argument("You can't define an item with id '" + id
      + "'. It is already defined by item '" + existing->name() + "'.");
  }

  Item item(id, name, value, width, height, items);
  items.add(id, item);
  out << "OK\n";
}

inline void list_items(std::ostream& out, std::istream& in, ItemTable& items, CollectionTable& collections, Inventory& inventory)
{
  (void) in;
  (void) collections;
  (void) inventory;

  std::vector< const Item* > sorted;
  for (auto it = items.begin(); it != items.end(); ++it)
  {
    sorted.push_back(&(*it));
  }

  std::sort(sorted.begin(), sorted.end(), [](const Item* lhs, const Item* rhs)
  {
    return lhs -> id() < rhs -> id();
  });

  for (const Item* item : sorted)
  {
    out << "Item [" << item->id() << "]: \"" << item->name() << "\" ("
      << item->width() << " x " << item->height() << ", value = "
      << item->value() << ")\n";
  }
}

#endif
