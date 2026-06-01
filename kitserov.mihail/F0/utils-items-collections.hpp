#ifndef UTILS_ITEMS_COLLECTIONS_HPP
#define UTILS_ITEMS_COLLECTIONS_HPP

#include "item.hpp"

#include <algorithm>
#include <istream>
#include <ostream>
#include <string>
#include <vector>

inline void define_item(std::ostream& out, std::istream& in, kitserov::ItemTable& items,
  kitserov::CollectionTable& collections, kitserov::InventoryTable& inventories)
{
  std::string id;
  std::string name;
  size_t width = 0;
  size_t height = 0;
  size_t value = 0;
  (void) collections;
  (void) inventories;

  in >> id >> name >> width >> height >> value;

  const kitserov::Item* existing = items.find(id);
  if (existing != nullptr)
  {
    throw std::invalid_argument("You can't define an item with id '" + id
      + "'. It is already defined by item '" + existing->name() + "'.");
  }

  kitserov::Item item(id, name, value, width, height, items);
  items.add(id, item);
  out << "OK\n";
}

inline void list_items(std::ostream& out, std::istream& in, kitserov::ItemTable& items,
  kitserov::CollectionTable& collections, kitserov::InventoryTable& inventories)
{
  (void) in;
  (void) collections;
  (void) inventories;

  std::vector< const kitserov::Item* > sorted;
  for (auto it = items.begin(); it != items.end(); ++it)
  {
    sorted.push_back(&(*it));
  }

  std::sort(sorted.begin(), sorted.end(), [](const kitserov::Item* lhs, const kitserov::Item* rhs)
  {
    return lhs->id() < rhs->id();
  });

  for (const kitserov::Item* item : sorted)
  {
    out << "Item [" << item->id() << "]: \"" << item->name() << "\" ("
      << item->width() << " x " << item->height() << ", value = "
      << item->value() << ")\n";
  }
}

inline void create_collection(std::ostream& out, std::istream& in, kitserov::ItemTable& items,
  kitserov::CollectionTable& collections, kitserov::InventoryTable& inventories)
{
  (void) items;
  (void) inventories;

  std::string collectionName;
  in >> collectionName;

  if (collections.contains(collectionName))
  {
    throw std::invalid_argument("collection " + collectionName + " already define");
  }

  collections.add(collectionName, kitserov::ItemCollection(20));
  out << "OK\n";
}

inline void add(std::ostream& out, std::istream& in, kitserov::ItemTable& items,
  kitserov::CollectionTable& collections, kitserov::InventoryTable& inventories)
{
  (void) out;
  (void) inventories;

  std::string collectionName;
  std::string itemId;
  long long amount = 0;
  in >> collectionName >> itemId >> amount;

  kitserov::ItemCollection* collection = collections.find(collectionName);
  if (collection == nullptr)
  {
    throw std::invalid_argument("collection " + collectionName + " have not defined");
  }

  if (amount <= 0)
  {
    throw std::invalid_argument("count for add must be positive");
  }

  const kitserov::Item* item = items.find(itemId);
  if (item == nullptr)
  {
    throw std::invalid_argument("Item with id " + itemId + " not defined.");
  }

  (void) item;

  size_t* existingAmount = collection->find(itemId);
  if (existingAmount == nullptr)
  {
    collection->add(itemId, static_cast< size_t >(amount));
  }
  else
  {
    *existingAmount += static_cast< size_t >(amount);
  }

  out << "OK\n";
}

inline void show_collection(std::ostream& out, std::istream& in, kitserov::ItemTable& items,
  kitserov::CollectionTable& collections, kitserov::InventoryTable& inventories)
{
  (void) inventories;

  std::string collectionName;
  in >> collectionName;

  const kitserov::ItemCollection* collection = collections.find(collectionName);
  if (collection == nullptr)
  {
    throw std::invalid_argument("collection " + collectionName + " have not defined");
  }

  std::vector< std::pair< std::string, size_t > > sorted;
  for (auto it = collection->begin(); it != collection->end(); ++it)
  {
    sorted.push_back({it.key(), *it});
  }

  std::sort(sorted.begin(), sorted.end(), [](const std::pair< std::string, size_t >& lhs,
    const std::pair< std::string, size_t >& rhs)
  {
    return lhs.first < rhs.first;
  });

  out << collectionName << ":\n";
  for (const auto& entry : sorted)
  {
    const kitserov::Item* item = items.find(entry.first);
    if (item == nullptr)
    {
      continue;
    }
    out << item->name() << ": " << entry.second
      << " (value = " << entry.second * item->value() << ")\n";
  }
}

#endif
