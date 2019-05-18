#pragma once

#include "instance.h"

#include <map>
#include <vector>

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

class solutionfinder_bfs : public solutionfinder
{
public:
    solutionfinder_bfs(instance& ins) : solutionfinder(ins) {}

protected:
    void find_from_root(size_t root) override;
};

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
