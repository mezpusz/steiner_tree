
#include "inputparser.h"

#include <deque>
#include <iostream>
#include <fstream>
#include <sstream>
#include <set>
#include <cassert>
#include <queue>

class score_compare {
    instance& ins;
public:
    score_compare(instance& ins) : ins(ins) {}
    bool operator()(const size_t& lhs, const size_t& rhs) const {
        return ins.terminals.at(lhs).score > ins.terminals.at(rhs).score;
    }
};

using min_score_queue = std::priority_queue<size_t, std::vector<size_t>, score_compare>;

bool bfs(size_t t,
        instance& ins,
        const std::unordered_set<size_t>& actives_set,
        std::set<std::pair<size_t, size_t>>& list) {
    std::cout << "Checking " << t << std::endl;

    std::deque<size_t> q;
    q.push_back(t);
    std::set<size_t> cut;
    cut.insert(t);

    while (!q.empty()) {
        auto x = q.front();
        q.pop_front();

        // traverse saturated nodes
        for (const auto& a : ins.nodes[x].saturated) {
            if (actives_set.count(a) > 0) {
                std::cout << "Another active terminal was hit: " << a << std::endl;
                ins.terminals.at(t).component = a;
                return false;
            }
            if (cut.count(a) == 0) {
                cut.insert(a);
                q.push_back(a);
            }
        }
        // collect potential steiner cut nodes
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
    ins.terminals.at(t).score = list.size();
    return list.size() > 0;
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

void find_min_route(instance& ins) {

}

void print_min_tree (instance& ins) {
    for (auto it = ins.terminals.begin(); it != ins.terminals.end(); it++) {
        auto comp = it->second.component;
        if (comp == it->first) {
            size_t root = comp;
            std::deque<size_t> q;
            std::unordered_set<size_t> seen;
            seen.insert(root);
            q.push_back(root);
            auto term = ins.terminals;
            term.erase(root);
            size_t sum = 0;
            while (!q.empty()) {
                auto x = q.front();
                q.pop_front();
                for (const auto& [a, e] : ins.nodes[x].edges) {
                    if (e.capacity == 0 && seen.count(a) == 0) {
                        std::cout << x << "->" << a << std::endl;
                        sum += e.weight;
                        seen.insert(a);
                        term.erase(a);
                        q.push_back(a);
                    }
                }
            }
            std::cout << std::endl;
            if (term.empty()) {
                std::cout << "Sum is " << sum << std::endl;
                break;
            }
        }
    }
}

int dual_ascent(instance& ins) {
    min_score_queue actives(ins);
    std::unordered_set<size_t> actives_set;
    for (const auto& t : ins.terminals) {
        actives.push(t.first);
        actives_set.insert(t.first);
    }
    std::vector<std::pair<size_t, size_t>> saturated;

    while (!actives.empty()) {
        size_t t = actives.top();
        actives.pop();
        actives_set.erase(t);
        std::set<std::pair<size_t, size_t>> list;
        if (bfs(t, ins, actives_set, list)) {
            // there's no use in actives with no outgoing edges,
            // remove them
            while (ins.terminals.at(actives.top()).score == 0) {
                actives_set.erase(actives.top());
                actives.pop();
            }
            auto scoreMin = ins.terminals.at(actives.top()).score;
            auto scoreCurr = ins.terminals.at(t).score;
            if (scoreMin < scoreCurr) {
                std::cout << t << " was not selected "
                          << scoreMin << " (of " << actives.top()
                           << ") < " << scoreCurr << std::endl;
            } else {
                std::cout << t << " is selected" << std::endl;
                pick_minimal_edge(ins, list);
            }
            actives.push(t);
        }
        std::cout << std::endl;
    }
    print_min_tree(ins);
    return 0;
}

int main(int argc, char* argv[]) {
    std::string infile_str, outfile_str;
    infile_str = "B/b01.stp";
    outfile_str = "output.txt";
    auto instance = parse(infile_str);
    std::stringstream rootstr;
    std::cout << "Found " << instance.nodes.size() << " nodes and "
              << instance.terminals.size()
              << " terminals in input file" << std::endl;

    dual_ascent(instance);
    std::ofstream out(outfile_str);
    if (out.is_open()) {
        out.close();
    } else {
        return -1;
    }

    return 0;
}