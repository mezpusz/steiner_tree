#pragma once

#include <unordered_map>
#include <unordered_set>
#include <vector>

struct edge {
    edge(size_t w) : weight(w), capacity(w) {}
    size_t weight;
    size_t capacity;
};

struct node {
    std::unordered_map<size_t, edge> edges;
    std::unordered_map<size_t, bool> redges;
};

struct terminal {
    terminal(size_t s)
        : score(s) {}
    size_t score;
};

struct instance {
    std::vector<node> nodes;
    std::unordered_map<size_t, terminal> terminals;
};