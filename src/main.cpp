
#include "inputparser.h"
#include "solutionfinder.h"

#include <deque>
#include <iostream>
#include <fstream>
#include <sstream>
#include <set>
#include <cassert>
#include <queue>
#include <memory>

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
    // std::cout << "Checking " << t << std::endl;

    std::deque<size_t> q;
    q.push_back(t);
    std::set<size_t> cut;
    cut.insert(t);

    while (!q.empty()) {
        auto x = q.front();
        q.pop_front();

        // traverse saturated nodes
        for (const auto& [a, sat] : ins.nodes[x].redges) {
            if (sat) {
                if (actives_set.count(a) > 0) {
                    // std::cout << "Another active terminal was hit: " << a << std::endl;
                    return false;
                }
                if (cut.count(a) == 0) {
                    cut.insert(a);
                    q.push_back(a);
                }
            // collect potential steiner cut nodes
            } else if (cut.count(a) == 0) {
                list.insert(std::make_pair(a, x));
            }
        }
    }
    // std::cout << "List contains" << std::endl;
    for (auto it = list.begin(); it != list.end();) {
        if (cut.count(it->first) > 0) {
            it = list.erase(it);
        } else {
            // std::cout << it->first << " " << it->second << std::endl;
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
    // std::cout << "Minimal capacity is " << min << std::endl;
    for (const auto& e : edges) {
        auto edge = ins.nodes[e.first].edges.find(e.second);
        if (edge != ins.nodes[e.first].edges.end()) {
            assert(min <= edge->second.capacity);
            if (edge->second.capacity == min) {
                // std::cout << e.first << " " << e.second << " became saturated" << std::endl;
                ins.nodes[e.second].redges[e.first] = true;
            }
            edge->second.capacity -= min;
        }
    }
    return true;
}

void find_min_route(instance& ins) {

}

int dual_ascent(instance& ins) {
    min_score_queue actives(ins);
    std::unordered_set<size_t> actives_set;
    for (const auto& t : ins.terminals) {
        actives.push(t.first);
        actives_set.insert(t.first);
    }

    while (!actives.empty()) {
        size_t t = actives.top();
        actives.pop();
        actives_set.erase(t);
        std::set<std::pair<size_t, size_t>> list;
        if (bfs(t, ins, actives_set, list)) {
            // there's no use in actives with no steiner cut,
            // remove them
            while (ins.terminals.at(actives.top()).score == 0) {
                actives_set.erase(actives.top());
                actives.pop();
            }
            auto scoreMin = ins.terminals.at(actives.top()).score;
            auto scoreCurr = ins.terminals.at(t).score;
            if (scoreMin < scoreCurr) {
                // std::cout << t << " was not selected "
                //           << scoreMin << " (of " << actives.top()
                //            << ") < " << scoreCurr << std::endl;
            } else {
                // std::cout << t << " is selected" << std::endl;
                pick_minimal_edge(ins, list);
            }
            actives.push(t);
        }
        // std::cout << std::endl;
    }
    return 0;
}

int main(int argc, char* argv[]) {
    std::string infile_str, outfile_str;
    if (argc >= 2) {
        infile_str = argv[1];
    } else {
        infile_str = "B/b04.stp";
    }
    outfile_str = "output.txt";
    auto instance = parse(infile_str);
    std::stringstream rootstr;
    std::cout << "Found " << instance.nodes.size() << " nodes and "
              << instance.terminals.size()
              << " terminals in input file" << std::endl << std::endl;

    dual_ascent(instance);
    auto sf_bfs = std::make_unique<solutionfinder_bfs>(instance);
    sf_bfs->find();
    std::cout << "Bfs found solution with sum " << sf_bfs->get_min() << std::endl;
    auto sf_dfs = std::make_unique<solutionfinder_dfs>(instance);
    sf_dfs->find();
    std::cout << "Dfs found solution with sum " << sf_dfs->get_min() << std::endl;
    auto sf_dijkstra = std::make_unique<solutionfinder_dijkstra>(instance);
    sf_dijkstra->find();
    std::cout << "Dijkstra found solution with sum " << sf_dijkstra->get_min() << std::endl;
    std::ofstream out(outfile_str);
    if (out.is_open()) {
        out.close();
    } else {
        return -1;
    }

    return 0;
}