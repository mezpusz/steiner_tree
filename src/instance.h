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
    std::unordered_set<size_t> redges;
    std::unordered_set<size_t> saturated; // only backward edges
};

struct terminal {
    terminal(size_t c, size_t s)
        : component(c), score(s) {}
    size_t component;
    size_t score;
};

struct instance {
    std::vector<node> nodes;
    std::unordered_map<size_t, terminal> terminals;
};