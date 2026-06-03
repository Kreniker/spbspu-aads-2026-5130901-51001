#ifndef UTILS_INVENTORY_HPP
#define UTILS_INVENTORY_HPP

#include "item.hpp"

#include <algorithm>
#include <istream>
#include <ostream>
#include <string>

namespace kitserov
{
  namespace inventory_detail
  {
    struct PackingResult
    {
      size_t value_ = 0;
      Inventory inventory_;
    };

    inline bool can_place(const Inventory& inventory, const Item& item,
      size_t row, size_t col, bool rotated)
    {
      size_t itemRows = rotated ? item.width() : item.height();
      size_t itemCols = rotated ? item.height() : item.width();

      if (row + itemRows > inventory.rows() || col + itemCols > inventory.cols())
      {
        return false;
      }

      for (size_t currentRow = row; currentRow < row + itemRows; ++currentRow)
      {
        for (size_t currentCol = col; currentCol < col + itemCols; ++currentCol)
        {
          if (!inventory(currentRow, currentCol).id().empty())
          {
            return false;
          }
        }
      }

      return true;
    }

    inline void place_item(Inventory& inventory, const Item& item,
      size_t row, size_t col, bool rotated)
    {
      size_t itemRows = rotated ? item.width() : item.height();
      size_t itemCols = rotated ? item.height() : item.width();

      for (size_t currentRow = row; currentRow < row + itemRows; ++currentRow)
      {
        for (size_t currentCol = col; currentCol < col + itemCols; ++currentCol)
        {
          inventory(currentRow, currentCol) = item;
        }
      }
    }

    inline std::string occupancy_key(const Inventory& inventory)
    {
      std::string key;
      key.reserve(inventory.rows() * inventory.cols());
      for (size_t row = 0; row < inventory.rows(); ++row)
      {
        for (size_t col = 0; col < inventory.cols(); ++col)
        {
          key.push_back(inventory(row, col).id().empty() ? '0' : '1');
        }
      }
      return key;
    }

    inline Vector< const Item* > sort_items_by_value(Vector< const Item* > items)
    {
      std::sort(items.begin(), items.end(), [](const Item* lhs, const Item* rhs)
      {
        if (lhs -> value() != rhs -> value())
        {
          return lhs -> value() > rhs -> value();
        }

        size_t lhsArea = lhs -> width() * lhs -> height();
        size_t rhsArea = rhs -> width() * rhs -> height();
        if (lhsArea != rhsArea)
        {
          return lhsArea > rhsArea;
        }

        return lhs -> id() < rhs -> id();
      });
      return items;
    }

    using SeenTable = HashCucushka< std::string, size_t,
      std::hash< std::string >, std::hash< std::string >, std::equal_to< std::string > >;
    using CountTable = HashCucushka< std::string, std::pair< const Item*, size_t >,
      std::hash< std::string >, std::hash< std::string >, std::equal_to< std::string > >;

    inline void maximize_packing(const Vector< const Item* >& items, size_t index,
      const Vector< size_t >& suffixValue, Inventory& current, size_t currentValue,
      PackingResult& best, SeenTable& seen)
    {
      if (currentValue + suffixValue[index] <= best.value_)
      {
        return;
      }

      std::string key = std::to_string(index) + ':' + occupancy_key(current);
      size_t* position = seen.find(key);
      if (position != nullptr && *position >= currentValue)
      {
        return;
      }
      if (position == nullptr)
      {
        seen.add(key, currentValue);
      }
      else
      {
        *position = currentValue;
      }

      if (index == items.size())
      {
        if (currentValue > best.value_)
        {
          best.value_ = currentValue;
          best.inventory_ = current;
        }
        return;
      }

      const Item* item = items[index];

      maximize_packing(items, index + 1, suffixValue, current, currentValue, best, seen);

      for (size_t row = 0; row < current.rows(); ++row)
      {
        for (size_t col = 0; col < current.cols(); ++col)
        {
          if (can_place(current, *item, row, col, false))
          {
            Inventory next = current;
            place_item(next, *item, row, col, false);
            maximize_packing(items, index + 1, suffixValue, next,
              currentValue + item -> value(), best, seen);
          }

          if (item -> width() != item -> height() && can_place(current, *item, row, col, true))
          {
            Inventory next = current;
            place_item(next, *item, row, col, true);
            maximize_packing(items, index + 1, suffixValue, next,
              currentValue + item -> value(), best, seen);
          }
        }
      }
    }

    inline Vector< const Item* > expand_collection_items(const CollectionTable& collections,
      const ItemTable& items, const std::string& collectionName)
    {
      Vector< const Item* > result;
      const ItemCollection* collection = collections.find(collectionName);
      if (collection == nullptr)
      {
        throw std::invalid_argument("collection " + collectionName + " have not defined");
      }

      for (auto it = collection -> begin(); it != collection -> end(); ++it)
      {
        const Item* item = items.find(it.key());
        if (item == nullptr)
        {
          throw std::invalid_argument("Item with id " + it.key() + " not defined.");
        }

        for (size_t count = 0; count < *it; ++count)
        {
          result.pushBack(item);
        }
      }

      return result;
    }

    inline Vector< const Item* > expand_inventory_items(const Inventory& inventory)
    {
      CountTable counts;

      for (size_t row = 0; row < inventory.rows(); ++row)
      {
        for (size_t col = 0; col < inventory.cols(); ++col)
        {
          const Item& cell = inventory(row, col);
          if (cell.id().empty())
          {
            continue;
          }

          std::pair< const Item*, size_t >* entry = counts.find(cell.id());
          if (entry == nullptr)
          {
            counts.add(cell.id(), std::make_pair(&cell, size_t(1)));
            continue;
          }
          ++entry -> second;
        }
      }

      Vector< const Item* > result;
      for (auto it = counts.begin(); it != counts.end(); ++it)
      {
        const Item* item = it -> first;
        size_t area = item -> width() * item -> height();
        size_t copies = area == 0 ? 0 : it -> second / area;
        for (size_t index = 0; index < copies; ++index)
        {
          result.pushBack(item);
        }
      }

      return result;
    }

    inline Inventory pack_best(const Inventory& base, Vector< const Item* > items)
    {
      Inventory current = base;
      items = sort_items_by_value(std::move(items));

      Vector< size_t > suffixValue(items.size() + 1, 0);
      for (size_t index = items.size(); index > 0; --index)
      {
        suffixValue[index - 1] = suffixValue[index] + items[index - 1] -> value();
      }

      PackingResult best;
      best.inventory_ = base;
      SeenTable seen;
      maximize_packing(items, 0, suffixValue, current, 0, best, seen);
      return best.inventory_;
    }

    inline Inventory repack_inventory(const Inventory& source, size_t rows, size_t cols)
    {
      Inventory target(rows, cols);
      Vector< const Item* > items = expand_inventory_items(source);
      return pack_best(target, std::move(items));
    }

    inline Inventory place_collection_into_inventory(const Inventory& inventory,
      const CollectionTable& collections, const ItemTable& items,
      const std::string& collectionName)
    {
      Vector< const Item* > itemCopies = expand_collection_items(collections, items, collectionName);
      return pack_best(inventory, std::move(itemCopies));
    }
  }


  inline void create_inv(std::ostream& out, std::istream& in, ItemTable& items,
    CollectionTable& collections, InventoryTable& inventories)
  {
    (void) items;
    (void) collections;

    std::string invName;
    size_t width = 0;
    size_t height = 0;
    in >> invName >> width >> height;

    if (inventories.contains(invName))
    {
      Inventory* existing = inventories.find(invName);
      if (existing != nullptr)
      {
        out << "Are you want repack inventory \"" << invName << "\"?\n"
          << "Press Y if yes and ANY another key if no\n";
        std::string answer;
        if (in >> answer && (answer == "Y" || answer == "y")) {
          *existing = inventory_detail::repack_inventory(*existing, height, width);
          out << "OK\n";
        } else {
          out << "CANCELED\n";
        }
        return;
      }
    }

    inventories.add(invName, Inventory(height, width));
    out << "OK\n";
  }

  inline void show_inv(std::ostream& out, std::istream& in, ItemTable& items,
    CollectionTable& collections, InventoryTable& inventories)
  {
    (void) items;
    (void) collections;

    std::string invName;
    in >> invName;

    const Inventory* inventory = inventories.find(invName);
    if (inventory == nullptr)
    {
      throw std::invalid_argument("Inventory " + invName + " have not defined");
    }

    out << *inventory << '\n';
  }

  inline void place(std::ostream& out, std::istream& in, ItemTable& items,
    CollectionTable& collections, InventoryTable& inventories)
  {
    (void) collections;

    std::string invName;
    std::string itemId;
    long long x = 0;
    long long y = 0;
    in >> invName >> itemId >> x >> y;

    Inventory* inventory = inventories.find(invName);
    if (inventory == nullptr)
    {
      throw std::invalid_argument("Inventory " + invName + " have not defined");
    }

    const Item* item = items.find(itemId);
    if (item == nullptr)
    {
      throw std::invalid_argument("Item with id " + itemId + " not defined.");
    }

    if (x < 0 || static_cast< size_t >(x) >= inventory -> cols())
    {
      throw std::invalid_argument("x is out of range.");
    }

    if (y < 0 || static_cast< size_t >(y) >= inventory -> rows())
    {
      throw std::invalid_argument("y is out of range.");
    }

    size_t rowIndex = static_cast< size_t >(y);
    size_t colIndex = static_cast< size_t >(x);

    if (inventory_detail::can_place(*inventory, *item, rowIndex, colIndex, false))
    {
      inventory_detail::place_item(*inventory, *item, rowIndex, colIndex, false);
      out << "OK\n";
      return;
    }

    if (inventory_detail::can_place(*inventory, *item, rowIndex, colIndex, true))
    {
      inventory_detail::place_item(*inventory, *item, rowIndex, colIndex, true);
      out << "OK\n";
      return;
    }

    throw std::invalid_argument("There is not enough space in the inventory.");
  }

  inline void place_collection(std::ostream& out, std::istream& in, ItemTable& items,
    CollectionTable& collections, InventoryTable& inventories)
  {
    std::string invName;
    std::string collectionName;
    in >> invName >> collectionName;

    Inventory* inventory = inventories.find(invName);
    if (inventory == nullptr)
    {
      throw std::invalid_argument("Inventory " + invName + " have not defined");
    }

    if (!collections.contains(collectionName))
    {
      throw std::invalid_argument("collection " + collectionName + " have not defined");
    }

    *inventory = inventory_detail::place_collection_into_inventory(
      *inventory, collections, items, collectionName);
    out << "OK\n";
  }
  inline void clear(std::ostream& out, std::istream& in, ItemTable& items,
    CollectionTable& collections, InventoryTable& inventories)
  {
    (void) collections;
    (void) items;
    std::string invName;
    in >> invName;
    Inventory* inventory = inventories.find(invName);
    if (inventory == nullptr)
    {
      throw std::invalid_argument("Inventory " + invName + " have not defined");
    }
    for (size_t i = 0; i < inventory -> cols(); ++i)
    {
      for (size_t j = 0; j < inventory -> rows(); ++j)
      {
        (*inventory)(j, i) = Item();
      }
    }
    out << "Are you want delete this inventory?\n"
      << "Press Y if yes and ANY another key if no\n";
    std::string answer;
    if (in >> answer && (answer == "Y" || answer == "y")) {
      inventory -> clear();
      inventories.erase(invName);
    }
    out << "OK\n";
  }
  inline void find_space(std::ostream& out, std::istream& in, ItemTable& items,
    CollectionTable& collections, InventoryTable& inventories)
  {
    (void) collections;
    std::string invName;
    std::string itemId;
    in >> invName >> itemId;
    Inventory* inventory = inventories.find(invName);
    if (inventory == nullptr)
    {
      throw std::invalid_argument("Inventory " + invName + " have not defined");
    }

    const Item* item = items.find(itemId);
    if (item == nullptr)
    {
      throw std::invalid_argument("Item with id " + itemId + " not defined.");
    }
    for (size_t rowIndex = 0; rowIndex < inventory -> rows(); ++rowIndex)
    {
      for (size_t colIndex = 0; colIndex < inventory -> cols(); ++colIndex)
      {
        if (inventory_detail::can_place(*inventory, *item, rowIndex, colIndex, false)
          || inventory_detail::can_place(*inventory, *item, rowIndex, colIndex, true))
        {
          out << "YES\n"
            << "x:" << colIndex << "\n"
            << "y:" << rowIndex << "\n";
          return;
        }
      }
    }
    out << "NO\n";
  }
  inline void organize(std::ostream& out, std::istream& in, ItemTable& items,
    CollectionTable& collections, InventoryTable& inventories)
  {
    (void) items;
    (void) collections;
    std::string invName;
    in >> invName;
    Inventory* inventory = inventories.find(invName);
    if (inventory == nullptr)
    {
      throw std::invalid_argument("Inventory " + invName + " have not defined");
    }
    Vector< const Item* > itemsFromInv = inventory_detail::expand_inventory_items(*inventory);
    Inventory empty(inventory -> rows(), inventory -> cols());
    *inventory = inventory_detail::pack_best(empty, std::move(itemsFromInv));
    out << "OK\n";
  }
}
#endif
