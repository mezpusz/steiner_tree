#pragma once

#include <map>
#include <vector>

struct instance {
    size_t nodes;
    std::map<std::pair<size_t, size_t>, size_t> edges;
    std::vector<size_t> terminals;
};