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
    ItemTable& items, CollectionTable& collections, InventoryTable& inventories);
  HashCucushka< std::string, CommandHandler,
    std::hash< std::string >, std::hash< std::string >,
    std::equal_to< std::string > > cmds(20);
  cmds.add("define-item", define_item);
  cmds.add("define_item", define_item);
  cmds.add("list-items", list_items);
  cmds.add("list_items", list_items);
  cmds.add("create-collection", create_collection);
  cmds.add("create_collection", create_collection);
  cmds.add("list-collections", list_collections);
  cmds.add("list_collections", list_collections);
  cmds.add("add", add);
  cmds.add("show-collection", show_collection);
  cmds.add("show_collection", show_collection);
  cmds.add("create-inv", create_inv);
  cmds.add("create_inv", create_inv);
  cmds.add("list-inv", list_inv);
  cmds.add("list_inv", list_inv);
  cmds.add("show-inv", show_inv);
  cmds.add("show_inv", show_inv);
  cmds.add("place", place);
  cmds.add("place-collection", place_collection);
  cmds.add("place_collection", place_collection);
  cmds.add("help", help);
  cmds.add("save", save);
  cmds.add("load", load);
  ItemTable items(20);
  CollectionTable collections(20);
  InventoryTable inventories(20);

  std::string cmd;
  while (std::cin >> cmd)
  {
    if (cmd == "exit" || cmd == "quit")
    {
      std::cout << "Bye!\n";
      break;
    }
    try
    {
      auto handler = cmds.find(cmd);
      if (!handler)
      {
        throw std::invalid_argument("unknown command");
      }
      (*handler)(std::cout, std::cin, items, collections, inventories);
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
