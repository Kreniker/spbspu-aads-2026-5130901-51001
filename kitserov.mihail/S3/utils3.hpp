#ifndef UTILS3_HPP
#define UTILS3_HPP

#include <vector>
#include <string>
#include <algorithm>
#include <stdexcept>
#include <istream>
#include "graph.hpp"

namespace kitserov
{

using HashGraphs = HashTable<std::string, Graph,
                              SipHash<std::string>,
                              std::equal_to<std::string> >;

void cmd_graphs(std::ostream& out, std::istream&, HashGraphs& graphs)
{
  std::vector<std::string> tmp;
  for (auto it = graphs.begin(); it != graphs.end(); ++it)
  {
    tmp.push_back(it.key());
  }
  std::sort(tmp.begin(), tmp.end());
  for (const auto& i : tmp)
  {
    out << i << "\n";
  }
}

void cmd_vertexes(std::ostream& out, std::istream& in, HashGraphs& graphs)
{
  std::string graphName;
  if (!(in >> graphName))
  {
    throw std::invalid_argument("invalid graph name");
  }
  Graph* graph = graphs.find(graphName);
  if (!graph)
  {
    throw std::invalid_argument("graph not found: " + graphName);
  }
  std::vector<std::string> vertices = graph->getVertices();
  for (const auto& v : vertices)
  {
    out << v << "\n";
  }
}

void cmd_outbound(std::ostream& out, std::istream& in, HashGraphs& graphs)
{
  std::string graphName;
  std::string vertex;
  if (!(in >> graphName >> vertex))
  {
    throw std::invalid_argument("invalid graph name or vertex");
  }
  Graph* graph = graphs.find(graphName);
  if (!graph || !(graph->hasVertex(vertex)))
  {
    throw std::invalid_argument("graph or vertex not found");
  }
  auto result = graph->getOutbound(vertex);
  for (const auto& p : result)
  {
    out << p.first;
    for (size_t w : p.second)
    {
      out << ' ' << w;
    }
    out << '\n';
  }
}

void cmd_inbound(std::ostream& out, std::istream& in, HashGraphs& graphs)
{
  std::string graphName;
  std::string vertex;
  if (!(in >> graphName >> vertex))
  {
    throw std::invalid_argument("invalid graph name or vertex");
  }
  Graph* graph = graphs.find(graphName);
  if (!graph || !(graph->hasVertex(vertex)))
  {
    throw std::invalid_argument("graph or vertex not found");
  }
  auto result = graph->getInbound(vertex);
  for (const auto& p : result)
  {
    out << p.first;
    for (size_t w : p.second)
    {
      out << ' ' << w;
    }
    out << '\n';
  }
}

void cmd_bind(std::ostream&, std::istream& in, HashGraphs& graphs)
{
  std::string graphName;
  std::string src;
  std::string dst;
  size_t weight;
  if (!(in >> graphName >> src >> dst >> weight))
  {
    throw std::invalid_argument("invalid arguments for bind");
  }
  Graph* graph = graphs.find(graphName);
  if (!graph)
  {
    throw std::invalid_argument("graph not found: " + graphName);
  }
  graph->addEdge(src, dst, weight);
}

void cmd_cut(std::ostream&, std::istream& in, HashGraphs& graphs)
{
  std::string graphName;
  std::string src;
  std::string dst;
  size_t weight;
  if (!(in >> graphName >> src >> dst >> weight))
  {
    throw std::invalid_argument("invalid arguments for cut");
  }
  Graph* graph = graphs.find(graphName);
  if (!graph)
  {
    throw std::invalid_argument("graph not found: " + graphName);
  }
  if (!(graph->cut(src, dst, weight)))
  {
    throw std::runtime_error("cut failed: edge not found");
  }
}

void cmd_create(std::ostream&, std::istream& in, HashGraphs& graphs)
{
  std::string graphName;
  size_t count;
  if (!(in >> graphName))
  {
    throw std::invalid_argument("invalid arguments for create");
  }
  if (graphs.find(graphName))
  {
    throw std::invalid_argument("graph already exists: " + graphName);
  }
  if (!(in >> count))
  {
    throw std::invalid_argument("invalid arguments for create");
  }
  std::vector< std::string > vertices(count);
  for (size_t i = 0; i < count; ++i)
  {
    if (!(in >> vertices[i]))
    {
      throw std::invalid_argument("invalid vertex name");
    }
  }
  graphs.add(graphName, Graph::create(graphName, vertices));
}

void cmd_merge(std::ostream&, std::istream& in, HashGraphs& graphs)
{
  std::string newName;
  std::string old1;
  std::string old2;
  if (!(in >> newName >> old1 >> old2))
  {
    throw std::invalid_argument("invalid arguments for merge");
  }
  if (graphs.find(newName))
  {
    throw std::invalid_argument("graph already exists: " + newName);
  }
  Graph* g1 = graphs.find(old1);
  Graph* g2 = graphs.find(old2);
  if (!g1 || !g2)
  {
    throw std::invalid_argument("source graph not found");
  }
  graphs.add(newName, Graph::merge(newName, *g1, *g2));
}

void cmd_extract(std::ostream&, std::istream& in, HashGraphs& graphs)
{
  std::string newName;
  std::string oldName;
  size_t count;
  if (!(in >> newName >> oldName >> count))
  {
    throw std::invalid_argument("invalid arguments for extract");
  }
  if (graphs.find(newName))
  {
    throw std::invalid_argument("graph already exists: " + newName);
  }
  Graph* oldGraph = graphs.find(oldName);
  if (!oldGraph)
  {
    throw std::invalid_argument("source graph not found: " + oldName);
  }
  std::vector<std::string> vertices(count);
  for (size_t i = 0; i < count; ++i)
  {
    if (!(in >> vertices[i]))
    {
      throw std::invalid_argument("invalid vertex name");
    }
  }
  for (const auto& v : vertices)
  {
    if (!(oldGraph->hasVertex(v)))
    {
      throw std::invalid_argument("vertex not found: " + v);
    }
  }
  graphs.add(newName, Graph::extract(*oldGraph, newName, vertices));
}

}

#endif
