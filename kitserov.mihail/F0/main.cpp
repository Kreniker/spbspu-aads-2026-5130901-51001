#include "hash-cucushka.hpp"
#include "identity-hash.hpp"
#include "item.hpp"
#include "utils.hpp"
#include <string>
#include <iostream>
#include <functional>
#include <limits>
#include <stdexcept>

int main()
{
  using namespace kitserov;
  using CommandHandler = void (*)(std::ostream& out, std::istream& in,
    ItemTable& items, CollectionTable& collections, Inventory& inventory);
  HashCucushka< std::string, CommandHandler,
    std::hash< std::string >, std::hash< std::string >,
    std::equal_to< std::string > > cmds(20);
  cmds.add("define-item", define_item);
  cmds.add("list-items", list_items);
  cmds.add("create-collection", create_collection);
  cmds.add("add", add);
  cmds.add("show-collection", show_collection);
  ItemTable items(20);
  CollectionTable collections(20);
  Inventory inventory;

  std::string cmd;
  while (std::cin >> cmd)
  {
    try
    {
      auto handler = cmds.find(cmd);
      if (!handler)
      {
        throw std::invalid_argument("unknown command");
      }
      (*handler)(std::cout, std::cin, items, collections, inventory);
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
    catch (const std::exception& e)
    {
      std::cout << "Error: " << e.what() << '\n';
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }

  return 0;
}
