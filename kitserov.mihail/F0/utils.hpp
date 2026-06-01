#ifndef UTILS_HPP
#define UTILS_HPP

#include "item.hpp"

#include <algorithm>
#include <fstream>
#include <istream>
#include <ostream>
#include <string>
#include <vector>

namespace
{
  const char* const EMPTY_CELL = "-";

  bool can_place(const kitserov::Inventory& inventory, const kitserov::Item& item,
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

  void place_item(kitserov::Inventory& inventory, const kitserov::Item& item,
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

  bool place_at_first_fit(kitserov::Inventory& inventory, const kitserov::Item& item)
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

  std::vector< const kitserov::Item* > unique_items_from_inventory(const kitserov::Inventory& inventory)
  {
    std::vector< const kitserov::Item* > result;
    std::vector< std::string > seen;

    for (size_t row = 0; row < inventory.rows(); ++row)
    {
      for (size_t col = 0; col < inventory.cols(); ++col)
      {
        const kitserov::Item& cell = inventory(row, col);
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

  kitserov::Inventory repack_inventory(const kitserov::Inventory& source, size_t rows, size_t cols)
  {
    kitserov::Inventory target(rows, cols);
    std::vector< const kitserov::Item* > items = unique_items_from_inventory(source);

    std::sort(items.begin(), items.end(), [](const kitserov::Item* lhs, const kitserov::Item* rhs)
    {
      if (lhs->value() != rhs->value())
      {
        return lhs->value() > rhs->value();
      }

      return lhs->id() < rhs->id();
    });

    for (const kitserov::Item* item : items)
    {
      (void) place_at_first_fit(target, *item);
    }

    return target;
  }

  void writeItemSection(std::ostream& out, const kitserov::ItemTable& items)
  {
    std::vector< const kitserov::Item* > sorted;
    for (auto it = items.begin(); it != items.end(); ++it)
    {
      sorted.push_back(&(*it));
    }

    std::sort(sorted.begin(), sorted.end(), [](const kitserov::Item* lhs, const kitserov::Item* rhs)
    {
      return lhs->id() < rhs->id();
    });

    out << sorted.size() << '\n';
    for (const kitserov::Item* item : sorted)
    {
      out << item->id() << ' ' << item->name() << ' ' << item->width() << ' '
        << item->height() << ' ' << item->value() << '\n';
    }
  }

  void writeCollectionSection(std::ostream& out, const kitserov::CollectionTable& collections)
  {
    std::vector< std::pair< std::string, const kitserov::ItemCollection* > > sorted;
    for (auto it = collections.begin(); it != collections.end(); ++it)
    {
      sorted.push_back({it.key(), &(*it)});
    }

    std::sort(sorted.begin(), sorted.end(), [](const std::pair< std::string, const kitserov::ItemCollection* >& lhs,
      const std::pair< std::string, const kitserov::ItemCollection* >& rhs)
    {
      return lhs.first < rhs.first;
    });

    out << sorted.size() << '\n';
    for (const auto& entry : sorted)
    {
      out << entry.first << ' ' << entry.second->size();
      for (auto it = entry.second->begin(); it != entry.second->end(); ++it)
      {
        out << ' ' << it.key() << ' ' << *it;
      }
      out << '\n';
    }
  }

  void writeInventorySection(std::ostream& out, const kitserov::InventoryTable& inventories)
  {
    std::vector< std::pair< std::string, const kitserov::Inventory* > > sorted;
    for (auto it = inventories.begin(); it != inventories.end(); ++it)
    {
      sorted.push_back({it.key(), &(*it)});
    }

    std::sort(sorted.begin(), sorted.end(), [](const std::pair< std::string, const kitserov::Inventory* >& lhs,
      const std::pair< std::string, const kitserov::Inventory* >& rhs)
    {
      return lhs.first < rhs.first;
    });

    out << sorted.size() << '\n';
    for (const auto& entry : sorted)
    {
      const kitserov::Inventory& inventory = *entry.second;
      out << entry.first << ' ' << inventory.rows() << ' ' << inventory.cols() << '\n';
      for (size_t row = 0; row < inventory.rows(); ++row)
      {
        for (size_t col = 0; col < inventory.cols(); ++col)
        {
          if (col != 0)
          {
            out << ' ';
          }
          const std::string& id = inventory(row, col).id();
          out << (id.empty() ? EMPTY_CELL : id);
        }
        out << '\n';
      }
    }
  }

  bool readItemSection(std::istream& in, kitserov::ItemTable& items)
  {
    size_t count = 0;
    if (!(in >> count))
    {
      return false;
    }

    for (size_t index = 0; index < count; ++index)
    {
      std::string id;
      std::string name;
      size_t width = 0;
      size_t height = 0;
      size_t value = 0;
      in >> id >> name >> width >> height >> value;
      items.add(id, kitserov::Item(id, name, value, width, height, items));
    }

    return true;
  }

  bool readCollectionSection(std::istream& in, kitserov::CollectionTable& collections)
  {
    size_t count = 0;
    in >> count;
    for (size_t index = 0; index < count; ++index)
    {
      std::string name;
      size_t itemCount = 0;
      in >> name >> itemCount;
      kitserov::ItemCollection collection(20);
      for (size_t itemIndex = 0; itemIndex < itemCount; ++itemIndex)
      {
        std::string itemId;
        size_t amount = 0;
        in >> itemId >> amount;
        collection.add(itemId, amount);
      }
      collections.add(name, collection);
    }

    return true;
  }

  bool readInventorySection(std::istream& in, kitserov::InventoryTable& inventories,
    const kitserov::ItemTable& items)
  {
    size_t count = 0;
    in >> count;
    for (size_t index = 0; index < count; ++index)
    {
      std::string name;
      size_t rows = 0;
      size_t cols = 0;
      in >> name >> rows >> cols;

      kitserov::Inventory inventory(rows, cols);
      for (size_t row = 0; row < rows; ++row)
      {
        for (size_t col = 0; col < cols; ++col)
        {
          std::string itemId;
          in >> itemId;
          if (itemId == EMPTY_CELL)
          {
            continue;
          }

          const kitserov::Item* item = items.find(itemId);
          if (item != nullptr)
          {
            inventory(row, col) = *item;
          }
        }
      }

      inventories.add(name, inventory);
    }

    return true;
  }
}

using namespace kitserov;

inline void define_item(std::ostream& out, std::istream& in, ItemTable& items,
  CollectionTable& collections, InventoryTable& inventories)
{
  std::string id;
  std::string name;
  size_t width = 0;
  size_t height = 0;
  size_t value = 0;
  (void) collections;
  (void) inventories;

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

inline void list_items(std::ostream& out, std::istream& in, ItemTable& items,
  CollectionTable& collections, InventoryTable& inventories)
{
  (void) in;
  (void) collections;
  (void) inventories;

  std::vector< const Item* > sorted;
  for (auto it = items.begin(); it != items.end(); ++it)
  {
    sorted.push_back(&(*it));
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
}

inline void create_collection(std::ostream& out, std::istream& in, ItemTable& items,
  CollectionTable& collections, InventoryTable& inventories)
{
  (void) items;
  (void) inventories;

  std::string collectionName;
  in >> collectionName;

  if (collections.contains(collectionName))
  {
    throw std::invalid_argument("collection " + collectionName + " already define");
  }

  collections.add(collectionName, ItemCollection(20));
  out << "OK\n";
}

inline void add(std::ostream& out, std::istream& in, ItemTable& items,
  CollectionTable& collections, InventoryTable& inventories)
{
  (void) out;
  (void) inventories;

  std::string collectionName;
  std::string itemId;
  long long amount = 0;
  in >> collectionName >> itemId >> amount;

  ItemCollection* collection = collections.find(collectionName);
  if (collection == nullptr)
  {
    throw std::invalid_argument("collection " + collectionName + " have not defined");
  }

  if (amount <= 0)
  {
    throw std::invalid_argument("count for add must be positive");
  }

  const Item* item = items.find(itemId);
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

inline void show_collection(std::ostream& out, std::istream& in, ItemTable& items,
  CollectionTable& collections, InventoryTable& inventories)
{
  (void) inventories;

  std::string collectionName;
  in >> collectionName;

  const ItemCollection* collection = collections.find(collectionName);
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
    const Item* item = items.find(entry.first);
    if (item == nullptr)
    {
      continue;
    }
    out << item->name() << ": " << entry.second
      << " (value = " << entry.second * item->value() << ")\n";
  }
}

inline void create_inv(std::ostream& out, std::istream& in, ItemTable& items,
  CollectionTable& collections, InventoryTable& inventories)
{
  (void) items;
  (void) collections;

  std::string invName;
  size_t rows = 0;
  size_t cols = 0;
  in >> invName >> rows >> cols;

  if (inventories.contains(invName))
  {
    Inventory* existing = inventories.find(invName);
    if (existing != nullptr)
    {
      *existing = repack_inventory(*existing, rows, cols);
      out << "OK\n";
      return;
    }
  }

  inventories.add(invName, Inventory(rows, cols));
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

  if (can_place(*inventory, *item, rowIndex, colIndex, false))
  {
    place_item(*inventory, *item, rowIndex, colIndex, false);
    out << "OK\n";
    return;
  }

  if (can_place(*inventory, *item, rowIndex, colIndex, true))
  {
    place_item(*inventory, *item, rowIndex, colIndex, true);
    out << "OK\n";
    return;
  }

  throw std::invalid_argument("There is not enough space in the inventory.");
}

inline void save(std::ostream& out, std::istream& in, ItemTable& items,
  CollectionTable& collections, InventoryTable& inventories)
{
  std::string fileName;
  in >> fileName;

  std::ofstream file(fileName);
  if (!file)
  {
    throw std::invalid_argument("cannot open file");
  }

  writeItemSection(file, items);
  writeCollectionSection(file, collections);
  writeInventorySection(file, inventories);
  out << "OK\n";
}

inline void load(std::ostream& out, std::istream& in, ItemTable& items,
  CollectionTable& collections, InventoryTable& inventories)
{
  std::string fileName;
  in >> fileName;

  std::ifstream file(fileName);
  if (!file)
  {
    throw std::invalid_argument("cannot open file");
  }

  ItemTable loadedItems(items.capacity());
  CollectionTable loadedCollections(collections.capacity());
  InventoryTable loadedInventories(inventories.capacity());

  if (!readItemSection(file, loadedItems))
  {
    throw std::invalid_argument("cannot read file");
  }
  if (!readCollectionSection(file, loadedCollections))
  {
    throw std::invalid_argument("cannot read file");
  }
  if (!readInventorySection(file, loadedInventories, loadedItems))
  {
    throw std::invalid_argument("cannot read file");
  }

  items = loadedItems;
  collections = loadedCollections;
  inventories = loadedInventories;
  out << "OK\n";
}

#endif
