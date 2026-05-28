#ifndef UTILS4_HPP
#define UTILS4_HPP

#include <string>
#include <iostream>
#include <functional>
#include <stdexcept>
#include "common/hash.hpp"
#include "bitree.hpp"

using TreesTable = kitserov::HashTable< std::string,
  kitserov::BSTree< int, std::string >,
  kitserov::SipHash< std::string >,
  std::equal_to< std::string >  >;

void printHandler(std::ostream& out, std::istream& in, TreesTable& trees)
{
  std::string name;
  if (!(in >> name))
  {
    throw std::invalid_argument("");
  }

  auto treePtr = trees.find(name);
  if (!treePtr || treePtr -> empty())
  {
    out << "<EMPTY>\n";
  }

  out << name;
  for (auto it = treePtr -> begin(); it != treePtr -> end(); ++it)
  {
    out << ' ' << it -> first << ' ' << it -> second;
  }
  out << '\n';
}

void complementHandler(std::ostream&, std::istream& in, TreesTable& trees)
{
  std::string newName, aName, bName;
  if (!(in >> newName >> aName >> bName))
  {
    throw std::invalid_argument("");
  }
  if (newName == aName || newName == bName) {
    throw std::invalid_argument("");
  }
  auto aPtr = trees.find(aName);
  auto bPtr = trees.find(bName);

  kitserov::BSTree< int, std::string > result;
  if (aPtr)
  {
    for (auto it = aPtr -> begin(); it != aPtr -> end(); ++it)
    {
      int key = it -> first;
      if (!bPtr || !bPtr -> contains(key))
      {
        result.push(key, it -> second);
      }
    }
  } else {
    throw std::invalid_argument("");
  }

  trees.add(newName, result);
}

void intersectHandler(std::ostream&, std::istream& in, TreesTable& trees)
{
  std::string newName, aName, bName;
  if (!(in >> newName >> aName >> bName))
  {
    throw std::invalid_argument("");
  }

  auto aPtr = trees.find(aName);
  auto bPtr = trees.find(bName);

  kitserov::BSTree< int, std::string > result;
  if (aPtr && bPtr)
  {
    for (auto it = aPtr -> begin(); it != aPtr -> end(); ++it)
    {
      int key = it -> first;
      if (bPtr -> contains(key))
      {
        result.push(key, it -> second);
      }
    }
  } else {
    throw std::invalid_argument("");
  }

  trees.add(newName, result);
}

void unionHandler(std::ostream&, std::istream& in, TreesTable& trees)
{
  std::string newName, aName, bName;
  if (!(in >> newName >> aName >> bName))
  {
    throw std::invalid_argument("");
  }

  auto aPtr = trees.find(aName);
  auto bPtr = trees.find(bName);

  kitserov::BSTree< int, std::string > result;
  if (aPtr)
  {
    for (auto it = aPtr -> begin(); it != aPtr -> end(); ++it)
    {
      result.push(it -> first, it -> second);
    }
  } else {
    throw std::invalid_argument("");
  }
  if (bPtr)
  {
    for (auto it = bPtr -> begin(); it != bPtr -> end(); ++it)
    {
      if (!result.contains(it -> first))
      {
        result.push(it -> first, it -> second);
      }
    }
  } else {
    throw std::invalid_argument("");
  }

  trees.add(newName, result);
}

#endif
