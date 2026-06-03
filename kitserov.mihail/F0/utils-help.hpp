#ifndef UTILS_HELP_HPP
#define UTILS_HELP_HPP

#include "item.hpp"

#include <algorithm>
#include <istream>
#include <ostream>
#include <string>

namespace kitserov
{
  void help(std::ostream& out, std::istream&, ItemTable&,
    CollectionTable&, InventoryTable&)
  {
    out << "Available commands:\n"
      << "  define-item <id> <name> <width> <height> <value>\n"
      << "  list-items\n"
      << "  create-collection <collection_name>\n"
      << "  list-collections\n"
      << "  add <collection_name> <item_id> <amount>\n"
      << "  remove <collection_name> <item_id>  <count>\n"
      << "  show-collection <collection_name>\n"
      << "  create-inv <inventory_name> <cols> <rows>\n"
      << "  list-inv\n"
      << "  show-inv <inventory_name>\n"
      << "  place <inventory_name> <item_id> <x> <y>\n"
      << "  place-collection <inventory_name> <collection_name>\n"
      << "  remove <collection_name> <item_id>  <count>\n"
      << "  find-space <inv_name> <item_id>\n"
      << "  organize <inv_name>\n"
      << "  clear <inv_name>\n"
      << "  save <file_name>\n"
      << "  load <file_name>\n"
      << "  quit\n";
  }
  inline void list_items(std::ostream& out, std::istream& in, ItemTable& items,
    CollectionTable& collections, InventoryTable& inventories)
  {
    (void) in;
    (void) collections;
    (void) inventories;

    Vector< const Item* > sorted;
    for (auto it = items.begin(); it != items.end(); ++it)
    {
      sorted.pushBack(&(*it));
    }

    std::sort(sorted.begin(), sorted.end(), [](const Item* lhs, const Item* rhs)
    {
      return lhs->id() < rhs->id();
    });

    for (const Item* item : sorted)
    {
      out << "Item [" << item->id() << "]: \"" << item->name() << "\" ("
        << item->width() << " x " << item->height() << ", value = "
        << item->value() << ")\n";
    }
    if (sorted.size() == 0)
    {
      out << "No one item for now\n";
    }
  }
  inline void list_collections(std::ostream& out, std::istream& in, ItemTable& items,
    CollectionTable& collections, InventoryTable& inventories)
  {
    (void) in;
    (void) items;
    (void) inventories;

    for (auto it = collections.begin(); it != collections.end(); ++it)
    {
      out << "Collection \"" << it.key() << "\"\n";
    }
    if (collections.begin() == collections.end())
    {
      out << "No one collection for now\n";
    }
  }
  inline void list_inv(std::ostream& out, std::istream& in, ItemTable& items,
    CollectionTable& collections, InventoryTable& inventories)
  {
    (void) in;
    (void) items;
    (void) collections;

    for (auto it = inventories.begin(); it != inventories.end(); ++it)
    {
      out << "Inventory \"" << it.key() << "\" "
        << (*it).cols() << " x " << (*it).rows() <<"\n";
    }
    if (inventories.begin() == inventories.end())
    {
      out << "No one inventory for now\n";
    }
  }
}
#endif
