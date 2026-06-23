#ifndef UTILS_SAVE_LOAD_HPP
#define UTILS_SAVE_LOAD_HPP

#include "item.hpp"
#include "matrix.hpp"

#include <algorithm>
#include <fstream>
#include <istream>
#include <ostream>
#include <string>
#include <utility>

namespace kitserov
{
  namespace persistence_detail
  {
    inline void writeItemSection(std::ostream& out, const ItemTable& items)
    {
      Vector< const Item* > sorted;
      for (auto it = items.begin(); it != items.end(); ++it)
      {
        sorted.pushBack(&(*it));
      }

      std::sort(sorted.begin(), sorted.end(), [](const Item* lhs, const Item* rhs)
      {
        return lhs -> id() < rhs -> id();
      });

      out << sorted.size() << '\n';
      for (const Item* item : sorted)
      {
        out << item -> id() << ' ' << item -> name() << ' ' << item -> width() << ' '
          << item -> height() << ' ' << item -> value() << '\n';
      }
    }

    inline void writeCollectionSection(std::ostream& out, const CollectionTable& collections)
    {
      Vector< std::pair< std::string, const ItemCollection* > > sorted;
      for (auto it = collections.begin(); it != collections.end(); ++it)
      {
        sorted.pushBack({it.key(), &(*it)});
      }

      std::sort(sorted.begin(), sorted.end(), [](const std::pair< std::string, const ItemCollection* >& lhs,
        const std::pair< std::string, const ItemCollection* >& rhs)
      {
        return lhs.first < rhs.first;
      });

      out << sorted.size() << '\n';
      for (const auto& entry : sorted)
      {
        out << entry.first << ' ' << entry.second -> size();
        for (auto it = entry.second -> begin(); it != entry.second -> end(); ++it)
        {
          out << ' ' << it.key() << ' ' << *it;
        }
        out << '\n';
      }
    }

    inline void writeInventorySection(std::ostream& out, const InventoryTable& inventories)
    {
      Vector< std::pair< std::string, const Inventory* > > sorted;
      for (auto it = inventories.begin(); it != inventories.end(); ++it)
      {
        sorted.pushBack({it.key(), &(*it)});
      }

      std::sort(sorted.begin(), sorted.end(), [](const std::pair< std::string, const Inventory* >& lhs,
        const std::pair< std::string, const Inventory* >& rhs)
      {
        return lhs.first < rhs.first;
      });

      out << sorted.size() << '\n';
      for (const auto& entry : sorted)
      {
        const Inventory& inventory = *entry.second;
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
            out << (id.empty() ? "-" : id);
          }
          out << '\n';
        }
      }
    }

    inline bool readItemSection(std::istream& in, ItemTable& items)
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
        items.add(id, Item(id, name, value, width, height, items));
      }

      return true;
    }

    inline bool readCollectionSection(std::istream& in, CollectionTable& collections)
    {
      size_t count = 0;
      in >> count;
      for (size_t index = 0; index < count; ++index)
      {
        std::string name;
        size_t itemCount = 0;
        in >> name >> itemCount;
        ItemCollection collection(20);
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

    inline bool readInventorySection(std::istream& in, InventoryTable& inventories,
      const ItemTable& items)
    {
      size_t count = 0;
      in >> count;
      for (size_t index = 0; index < count; ++index)
      {
        std::string name;
        size_t rows = 0;
        size_t cols = 0;
        in >> name >> rows >> cols;

        Inventory inventory(rows, cols);
        for (size_t row = 0; row < rows; ++row)
        {
          for (size_t col = 0; col < cols; ++col)
          {
            std::string itemId;
            in >> itemId;
            if (itemId == "-")
            {
              continue;
            }

            const Item* item = items.find(itemId);
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

    persistence_detail::writeItemSection(file, items);
    persistence_detail::writeCollectionSection(file, collections);
    persistence_detail::writeInventorySection(file, inventories);
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

    if (!persistence_detail::readItemSection(file, loadedItems))
    {
      throw std::invalid_argument("cannot read file");
    }
    if (!persistence_detail::readCollectionSection(file, loadedCollections))
    {
      throw std::invalid_argument("cannot read file");
    }
    if (!persistence_detail::readInventorySection(file, loadedInventories, loadedItems))
    {
      throw std::invalid_argument("cannot read file");
    }

    items = loadedItems;
    collections = loadedCollections;
    inventories = loadedInventories;
    out << "OK\n";
  }
}
#endif
