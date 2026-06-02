#ifndef UTILS_HELP_HPP
#define UTILS_HELP_HPP

#include "item.hpp"

#include <algorithm>
#include <istream>
#include <ostream>
#include <string>
#include <vector>

namespace kitserov
{
  void help(std::ostream& out, std::istream&, ItemTable&,
    CollectionTable&, InventoryTable&)
  {
    out << "Available commands:\n"
      << "  define-item <id> <name> <width> <height> <value>\n"
      << "  list-items\n"
      << "  create-collection <collection_name>\n"
      << "  add <collection_name> <item_id> <amount>\n"
      << "  show-collection <collection_name>\n"
      << "  create-inv <inventory_name> <cols> <rows>\n"
      << "  show-inv <inventory_name>\n"
      << "  place <inventory_name> <item_id> <x> <y>\n"
      << "  place-collection <inventory_name> <collection_name>\n"
      << "  remove <collection_name> <item_id>  <count>\n"
      << "  find-space <inv_name> <item_id>\n"
      << "  organize <inv_name>\n"
      << "  clear <inv_name>\n"
      << "  save <file_name>\n"
      << "  load <file_name>\n";
  }
}
#endif
