#pragma once

#include "instance.h"

#include <map>
#include <vector>

// Abstract base class for different solution finder
// implementations. The idea is to traverse the saturated
// nodes from each of the terminals (assuming the root is
// not given/known) to see which one gives a minimal solution.
class solutionfinder {
public:
    solutionfinder(instance& ins);
    virtual ~solutionfinder() = default;

    void find();
    size_t get_min() const { return min; }
    std::vector<std::pair<size_t, size_t>> get_edges() const {
        return min_edges;
    }

protected:
    virtual void find_from_root(size_t root) = 0;

    instance& ins;
    size_t min;
    std::vector<std::pair<size_t, size_t>> min_edges;
};

// Finds a feasible solution by searching through
// the saturated graph breadth-first.
// Does not always give the best solution as we don't
// consider the weights thus there can be cheaper paths
// to each node.
class solutionfinder_bfs : public solutionfinder
{
public:
    solutionfinder_bfs(instance& ins) : solutionfinder(ins) {}

protected:
    void find_from_root(size_t root) override;
};

// Finds a feasible solution by searching through
// the saturated graph depth-first.
// Does not always give the best solution as we don't
// consider the weights thus there can be cheaper paths
// to each node.
class solutionfinder_dfs : public solutionfinder
{
public:
    solutionfinder_dfs(instance& ins) : solutionfinder(ins) {}

private:
    void find_from_root(size_t root) override;
    int dfs(size_t x,
        std::unordered_set<size_t>& seen,
        std::vector<std::pair<size_t,size_t>>& edges);
};

// Finds a feasible solution by giving shortest paths
// from the root to all nodes with Dijkstra's algorithm.
// Gives the minimal solution with respect to the approximated
// saturated graph.
class solutionfinder_dijkstra : public solutionfinder
{
public:
    solutionfinder_dijkstra(instance& ins) : solutionfinder(ins) {}

private:
    void find_from_root(size_t root) override;
    int dfs(size_t x,
        const std::map<size_t, int>& p,
        std::vector<std::pair<size_t,size_t>>& edges);
};
