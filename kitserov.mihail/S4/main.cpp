#include <fstream>
#include <functional>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>

#include "common/hash.hpp"
#include "bitree.hpp"
#include "utils4.hpp"

int main(int argc, char* argv[])
{
  using namespace kitserov;
  using TreesTable = HashTable< std::string,
    BSTree< int, std::string >,
    SipHash< std::string >,
    std::equal_to< std::string > >;
  using CommandHandler = void (*)(std::ostream& out, std::istream& in,
    TreesTable&);

  if (argc != 2)
  {
    std::cerr << "Usage: " << argv[0] << " <filename>\n";
    return 1;
  }

  std::ifstream inputFile(argv[1]);
  if (!inputFile)
  {
    std::cerr << "Error: cannot open file \"" << argv[1] << "\"\n";
    return 1;
  }

  TreesTable trees;
  std::string line;

  while (std::getline(inputFile, line))
  {
    if (line.empty())
    {
      continue;
    }

    const size_t firstSpace = line.find(' ');
    const std::string datasetName = line.substr(0, firstSpace);
    BSTree< int, std::string > tree;

    if (firstSpace != std::string::npos)
    {
      size_t position = firstSpace + 1;

      while (position < line.size())
      {
        const size_t secondSpace = line.find(' ', position);
        if (secondSpace == std::string::npos)
        {
          throw std::runtime_error("invalid dictionary file format");
        }

        const int key = std::stoi(line.substr(position, secondSpace - position));
        position = secondSpace + 1;

        const size_t thirdSpace = line.find(' ', position);
        const std::string value = line.substr(position, thirdSpace - position);
        tree.push(key, value);

        if (thirdSpace == std::string::npos)
        {
          break;
        }
        position = thirdSpace + 1;
      }
    }

    trees.add(datasetName, tree);
  }

  HashTable< std::string, CommandHandler, SipHash< std::string >,
             std::equal_to< std::string > > cmds(20);

  cmds.add("print", printHandler);
  cmds.add("complement", complementHandler);
  cmds.add("intersect", intersectHandler);
  cmds.add("union", unionHandler);

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
