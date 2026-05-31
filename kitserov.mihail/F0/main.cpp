#include "hash-cucushka.hpp"
#include "identity-hash.hpp"
#include "item.hpp"
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
    catch (...)
    {
      std::cout << "<INVALID COMMAND>\n";
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }

  return 0;
}
