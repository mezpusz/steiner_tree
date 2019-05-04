
#include "inputparser.h"

#include <deque>
#include <iostream>
#include <fstream>
#include <set>
#include <cassert>

bool bfs(size_t v,
        instance& ins,
        const std::unordered_set<size_t>& actives_set,
        std::set<std::pair<size_t, size_t>>& list) {
    std::cout << v << " is selected" << std::endl;
    std::deque<size_t> q;
    q.push_back(v);
    std::set<size_t> cut;
    cut.insert(v);
    while (!q.empty()) {
        auto x = q.front();
        q.pop_front();
        for (const auto& a : ins.nodes[x].saturated) {
            if (v != a && actives_set.count(a) > 0) {
                std::cout << "Another active terminal was hit: " << a << std::endl;
                return false;
            }
            if (cut.count(a) == 0) {
                cut.insert(a);
                q.push_back(a);
            }
        }
        for (const auto& a : ins.nodes[x].redges) {
            if (cut.count(a) == 0) {
                list.insert(std::make_pair(a, x));
            }
        }
    }
    std::cout << "List contains" << std::endl;
    for (auto it = list.begin(); it != list.end();) {
        if (cut.count(it->first) > 0) {
            it = list.erase(it);
        } else {
            std::cout << it->first << " " << it->second << std::endl;
            it++;
        }
    }
    return true;
}

bool pick_minimal_edge(instance& ins, std::set<std::pair<size_t, size_t>> edges) {
    if (edges.empty()) {
        return false;
    }
    size_t min = SIZE_MAX;
    for (const auto& e : edges) {
        auto edge = ins.nodes[e.first].edges.find(e.second);
        if (edge != ins.nodes[e.first].edges.end()) {
            auto cap = edge->second.capacity;
            if (cap > 0 && cap < min) {
                min = cap;
            }
        }
    }
    std::cout << "Minimal capacity is " << min << std::endl;
    for (const auto& e : edges) {
        auto edge = ins.nodes[e.first].edges.find(e.second);
        if (edge != ins.nodes[e.first].edges.end()) {
            assert(min <= edge->second.capacity);
            if (edge->second.capacity == min) {
                std::cout << e.first << " " << e.second << " became saturated" << std::endl;
                ins.nodes[e.second].saturated.insert(e.first);
            }
            edge->second.capacity -= min;
        }
    }
    return true;
}

void print_min_tree (instance& ins) {
    size_t root = 1;
    std::deque<size_t> q;
    std::unordered_set<size_t> seen;
    seen.insert(root);
    q.push_back(root);
    size_t sum = 0;
    while (!q.empty()) {
        auto x = q.front();
        q.pop_front();
        for (const auto& [a, e] : ins.nodes[x].edges) {
            if (e.capacity == 0 && seen.count(a) == 0) {
                std::cout << x << "->" << a << std::endl;
                sum += e.weight;
                seen.insert(a);
                q.push_back(a);
            }
        }
    }
    std::cout << "Sum is " << sum << std::endl;
}

int dual_ascent(instance& ins) {
    std::deque<size_t> actives;
    std::unordered_set<size_t> actives_set;
    for (const auto& t : ins.terminals) {
        actives.push_back(t);
        actives_set.insert(t);
    }
    std::vector<std::pair<size_t, size_t>> saturated;

    int i = 0;
    while (!actives.empty()) {
        if (i++ == 50) {
            break;
        }
        size_t t = actives.front();
        actives.pop_front();
        actives_set.erase(t);
        std::set<std::pair<size_t, size_t>> list;
        if (bfs(t, ins, actives_set, list) && pick_minimal_edge(ins, list)) {
            actives.push_back(t);
            actives_set.insert(t);
        }
        std::cout << std::endl << std::endl;
    }
    print_min_tree(ins);
    return 0;
}

int main(int argc, char* argv[]) {
    std::string infile_str, outfile_str;
    infile_str = "example";
    outfile_str = "output.txt";
    auto instance = parse(infile_str);
    std::cout << "Found " << instance.nodes.size() << " nodes, "
              << instance.terminals.size() << " terminals in input file" << std::endl;

    dual_ascent(instance);
    std::ofstream out(outfile_str);
    if (out.is_open()) {
        out.close();
    } else {
        return -1;
    }

    return 0;
}