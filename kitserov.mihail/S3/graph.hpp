#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <vector>
#include <algorithm>
#include <set>
#include <string>
#include <utility>
#include <stdexcept>
#include "hash.hpp"

namespace kitserov
{

class Graph
{
public:
  Graph() = default;

  explicit Graph(const std::string& n) :
    name_(n)
  {}

  void addEdge(const std::string& src, const std::string& dst, size_t weight)
  {
    vertices_.insert(src);
    vertices_.insert(dst);
    auto* vec = edges_.find({src, dst});
    if (vec)
    {
      vec->push_back(weight);
    }
    else
    {
      edges_.add({src, dst}, {weight});
    }
  }

  bool cut(const std::string& src, const std::string& dst, size_t weight)
  {
    std::vector<size_t>* point = edges_.find({src, dst});
    if (!point)
    {
      return false;
    }
    auto weightIt = std::find(point->begin(), point->end(), weight);
    if (weightIt == point->end())
    {
      return false;
    }
    point->erase(weightIt);
    if (point->empty())
    {
      edges_.erase({src, dst});
    }
    return true;
  }

  bool hasVertex(const std::string& v) const
  {
    return vertices_.find(v) != vertices_.end();
  }

  std::vector<std::string> getVertices() const
  {
    std::vector<std::string> v(vertices_.begin(), vertices_.end());
    std::sort(v.begin(), v.end());
    return v;
  }

  std::vector<std::pair<std::string, std::vector<size_t> > > getOutbound(const std::string& src) const
  {
    std::vector<std::pair<std::string, size_t> > flat;
    for (auto it = edges_.begin(); it != edges_.end(); ++it)
    {
      auto key = it.key();
      if (key.first == src)
      {
        const auto& weights = *it;
        for (size_t w : weights)
        {
          flat.emplace_back(key.second, w);
        }
      }
    }
    std::sort(flat.begin(), flat.end(),
      [](const auto& a, const auto& b)
      {
        if (a.first != b.first)
        {
          return a.first < b.first;
        }
        return a.second < b.second;
      });
    std::vector<std::pair<std::string, std::vector<size_t> > > result;
    for (size_t i = 0; i < flat.size();)
    {
      const std::string& vertex = flat[i].first;
      std::vector<size_t> weights;
      while (i < flat.size() && flat[i].first == vertex)
      {
        weights.push_back(flat[i].second);
        ++i;
      }
      result.emplace_back(vertex, std::move(weights));
    }
    return result;
  }

  std::vector<std::pair<std::string, std::vector<size_t> > > getInbound(const std::string& dst) const
  {
    std::vector<std::pair<std::string, size_t> > flat;
    for (auto it = edges_.begin(); it != edges_.end(); ++it)
    {
      auto key = it.key();
      if (key.second == dst)
      {
        const auto& weights = *it;
        for (size_t w : weights)
        {
          flat.emplace_back(key.first, w);
        }
      }
    }
    std::sort(flat.begin(), flat.end(),
      [](const auto& a, const auto& b)
      {
        if (a.first != b.first)
        {
          return a.first < b.first;
        }
        return a.second < b.second;
      });
    std::vector<std::pair<std::string, std::vector<size_t> > > result;
    for (size_t i = 0; i < flat.size();)
    {
      const std::string& vertex = flat[i].first;
      std::vector<size_t> weights;
      while (i < flat.size() && flat[i].first == vertex)
      {
        weights.push_back(flat[i].second);
        ++i;
      }
      result.emplace_back(vertex, std::move(weights));
    }
    return result;
  }

  static Graph create(const std::string& name, const std::vector<std::string>& vertices)
  {
    Graph g(name);
    for (const auto& v : vertices)
    {
      g.vertices_.insert(v);
    }
    return g;
  }

  static Graph merge(const std::string& name, const Graph& a, const Graph& b)
  {
    Graph g(name);
    g.vertices_ = a.vertices_;
    g.vertices_.insert(b.vertices_.begin(), b.vertices_.end());

    for (auto it = a.edges_.begin(); it != a.edges_.end(); ++it)
    {
      g.edges_.add(it.key(), *it);
    }
    for (auto it = b.edges_.begin(); it != b.edges_.end(); ++it)
    {
      auto key = it.key();
      auto* existing = g.edges_.find(key);
      if (existing)
      {
        existing->insert(existing->end(), it->begin(), it->end());
      }
      else
      {
        g.edges_.add(key, *it);
      }
    }
    return g;
  }

  static Graph extract(const Graph& src, const std::string& name,
                       const std::vector<std::string>& vertices)
  {
    Graph g(name);
    for (const auto& v : vertices)
    {
      if (!src.hasVertex(v))
      {
        throw std::invalid_argument("Vertex not found in source graph");
      }
      g.vertices_.insert(v);
    }
    for (auto it = src.edges_.begin(); it != src.edges_.end(); ++it)
    {
      auto key = it.key();
      const std::string& v1 = key.first;
      const std::string& v2 = key.second;
      if (g.vertices_.count(v1) && g.vertices_.count(v2))
      {
        g.edges_.add(key, *it);
      }
    }
    return g;
  }

private:
  using EdgeKey = std::pair<std::string, std::string>;
  using EdgeHash = PairHash<std::string, std::string, SipHash>;
  using EdgeEqual = std::equal_to<EdgeKey>;
  using EdgeTable = HashTable<EdgeKey, std::vector<size_t>, EdgeHash, EdgeEqual>;

  std::string name_;
  EdgeTable edges_;
  std::set<std::string> vertices_;
};

}

#endif
