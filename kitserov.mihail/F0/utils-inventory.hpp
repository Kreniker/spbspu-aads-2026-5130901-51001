#ifndef UTILS_INVENTORY_HPP
#define UTILS_INVENTORY_HPP

#include "item.hpp"

#include <algorithm>
#include <istream>
#include <ostream>
#include <string>
#include <vector>

namespace kitserov
{
  namespace inventory_detail
  {
    inline bool can_place(const Inventory& inventory, const Item& item, size_t row, size_t col, bool rotated)
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

    inline void place_item(Inventory& inventory, const Item& item, size_t row, size_t col, bool rotated)
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

    inline bool place_at_first_fit(Inventory& inventory, const Item& item)
    {
      for (size_t row = 0; row < inventory.rows(); ++row)
      {
        for (size_t col = 0; col < inventory.cols(); ++col)
        {
          if (can_place(inventory, item, row, col, false))
          {
            place_item(inventory, item, row, col, false);
            return true;
          }

          if (can_place(inventory, item, row, col, true))
          {
            place_item(inventory, item, row, col, true);
            return true;
          }
        }
      }

      return false;
    }

    inline std::vector< const Item* > unique_items_from_inventory(const Inventory& inventory)
    {
      std::vector< const Item* > result;
      std::vector< std::string > seen;

      for (size_t row = 0; row < inventory.rows(); ++row)
      {
        for (size_t col = 0; col < inventory.cols(); ++col)
        {
          const Item& cell = inventory(row, col);
          if (cell.id().empty())
          {
            continue;
          }

          if (std::find(seen.begin(), seen.end(), cell.id()) != seen.end())
          {
            continue;
          }

          seen.push_back(cell.id());
          result.push_back(&cell);
        }
      }

      return result;
    }

    inline Inventory repack_inventory(const Inventory& source, size_t rows, size_t cols)
    {
      Inventory target(rows, cols);
      std::vector< const Item* > items = unique_items_from_inventory(source);

      std::sort(items.begin(), items.end(), [](const Item* lhs, const Item* rhs)
      {
        if (lhs->value() != rhs->value())
        {
          return lhs->value() > rhs->value();
        }

        return lhs->id() < rhs->id();
      });

      for (const Item* item : items)
      {
        (void) place_at_first_fit(target, *item);
      }

      return target;
    }
  }
}

inline void create_inv(std::ostream& out, std::istream& in, kitserov::ItemTable& items,
  kitserov::CollectionTable& collections, kitserov::InventoryTable& inventories)
{
  (void) items;
  (void) collections;

  std::string invName;
  size_t rows = 0;
  size_t cols = 0;
  in >> invName >> rows >> cols;

  if (inventories.contains(invName))
  {
    kitserov::Inventory* existing = inventories.find(invName);
    if (existing != nullptr)
    {
      *existing = kitserov::inventory_detail::repack_inventory(*existing, rows, cols);
      out << "OK\n";
      return;
    }
  }

  inventories.add(invName, kitserov::Inventory(rows, cols));
  out << "OK\n";
}

inline void show_inv(std::ostream& out, std::istream& in, kitserov::ItemTable& items,
  kitserov::CollectionTable& collections, kitserov::InventoryTable& inventories)
{
  (void) items;
  (void) collections;

  std::string invName;
  in >> invName;

  const kitserov::Inventory* inventory = inventories.find(invName);
  if (inventory == nullptr)
  {
    throw std::invalid_argument("Inventory " + invName + " have not defined");
  }

  out << *inventory << '\n';
}

inline void place(std::ostream& out, std::istream& in, kitserov::ItemTable& items,
  kitserov::CollectionTable& collections, kitserov::InventoryTable& inventories)
{
  (void) collections;

  std::string invName;
  std::string itemId;
  long long x = 0;
  long long y = 0;
  in >> invName >> itemId >> x >> y;

  kitserov::Inventory* inventory = inventories.find(invName);
  if (inventory == nullptr)
  {
    throw std::invalid_argument("Inventory " + invName + " have not defined");
  }

  const kitserov::Item* item = items.find(itemId);
  if (item == nullptr)
  {
    throw std::invalid_argument("Item with id " + itemId + " not defined.");
  }

  if (x < 0 || static_cast< size_t >(x) >= inventory->cols())
  {
    throw std::invalid_argument("x is out of range.");
  }

  if (y < 0 || static_cast< size_t >(y) >= inventory->rows())
  {
    throw std::invalid_argument("y is out of range.");
  }

  size_t rowIndex = static_cast< size_t >(y);
  size_t colIndex = static_cast< size_t >(x);

  if (kitserov::inventory_detail::can_place(*inventory, *item, rowIndex, colIndex, false))
  {
    kitserov::inventory_detail::place_item(*inventory, *item, rowIndex, colIndex, false);
    out << "OK\n";
    return;
  }

  if (kitserov::inventory_detail::can_place(*inventory, *item, rowIndex, colIndex, true))
  {
    kitserov::inventory_detail::place_item(*inventory, *item, rowIndex, colIndex, true);
    out << "OK\n";
    return;
  }

  throw std::invalid_argument("There is not enough space in the inventory.");
}

#endif