#include "hash-cucushka.hpp"
#include "identity-hash.hpp"
#include <string>
#include <iostream>
#include <stdexcept>

int main()
{
  using namespace kitserov;
  using CommandHandler = void (*)(std::ostream& out, std::istream& in, Hash&);
  HashCucushka< std::string, CommandHandler,
    IdentityHash, IdentityHash, std::equal_to< std::string > > cmds(20);
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
      (*handler)(std::cout, std::cin, trees);
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
