
#include "solutionfinder.h"

#include <deque>
#include <set>

solutionfinder::solutionfinder(instance& ins)
    : ins(ins)
    , min(SIZE_MAX)
    , min_edges() {
}

void solutionfinder::find() {
    // consider all terminals as root of tree
    for (const auto& [u, t] : ins.terminals) {
        find_from_root(u);
    }
}

void solutionfinder_bfs::find_from_root(size_t root) {
    // simple bfs with double-ended queue
    // and set of traversed nodes
    std::deque<size_t> q;
    std::unordered_set<size_t> seen;
    seen.insert(root);
    q.push_back(root);
    auto term = ins.terminals;
    term.erase(root);
    size_t sum = 0;
    std::vector<std::pair<size_t, size_t>> edges;
    while (!q.empty()) {
        auto x = q.front();
        q.pop_front();
        for (const auto& [a, e] : ins.nodes[x].edges) {
            if (e.capacity == 0 && seen.count(a) == 0) {
                sum += e.weight;
                edges.push_back(std::make_pair(x, a));
                seen.insert(a);
                term.erase(a);
                q.push_back(a);
            }
        }
    }
    if (term.empty() && sum < min) {
        min = sum;
        min_edges = edges;
    }
}

int solutionfinder_dfs::dfs(size_t x,
        std::unordered_set<size_t>& seen,
        std::vector<std::pair<size_t,size_t>>& edges) {
    // Depth-first search with the trick that
    // we only count a 'dead-end' (i.e. a path
    // that ends in an already seen node) up to
    // the last terminal it contains.
    if (seen.count(x) == 0) {
        seen.insert(x);
    } else {
        return -1;
    }
    int sum = 0;
    // If this node is a terminal which hasn't
    // been processed, keep this path.
    // Otherwise see if any path from here
    // contains a terminal and keep it if yes.
    bool found_route = ins.terminals.count(x) > 0;
    for (const auto& [a, e] : ins.nodes[x].edges) {
        if (e.capacity == 0) {
            auto subsum = dfs(a, seen, edges);
            if (subsum != -1) {
                sum += e.weight + subsum;
                edges.push_back(std::make_pair(x, a));
                found_route = true;
            }
        }
    }
    if (!found_route) {
        sum = -1;
    }
    return sum;
}

void solutionfinder_dfs::find_from_root(size_t root) {
    std::unordered_set<size_t> seen;
    std::vector<std::pair<size_t,size_t>> edges;
    // simply call dfs then see if
    // all the terminals were covered
    auto sum = dfs(root, seen, edges);
    bool solution = true;
    for (const auto& [x,t] : ins.terminals) {
        if (seen.count(x) == 0) {
            solution = false;
            break;
        }
    }
    if (solution && sum < min) {
        min = sum;
        min_edges = edges;
    }
}

int solutionfinder_dijkstra::dfs(size_t x,
        const std::map<size_t, int>& p,
        std::vector<std::pair<size_t,size_t>>& edges) {
    // The DFS collecting the optimal tree after
    // the shortest paths were computed.
    // It only collects paths up to point they
    // contain a terminal.
    int sum = 0;
    bool found_route = ins.terminals.count(x) > 0;
    for (const auto& [a, e] : ins.nodes[x].edges) {
        if (e.capacity == 0 && p.at(a) == x) {
            auto subsum = dfs(a, p, edges);
            if (subsum != -1) {
                sum += e.weight + subsum;
                edges.push_back(std::make_pair(x, a));
                found_route = true;
            }
        }
    }
    if (!found_route) {
        sum = -1;
    }
    return sum;
}

void solutionfinder_dijkstra::find_from_root(size_t root) {
    auto term = ins.terminals;
    // priority queue for min dist elements
    std::set<std::pair<size_t, size_t>> actives;
    // node -> distance map
    std::map<size_t, size_t> dist;
    // stores processed nodes
    std::set<size_t> done;
    // parents map (int to store -1 of root)
    std::map<size_t, int> p;

    // init data structures
    actives.insert(std::make_pair(0, root));
    dist.insert(std::make_pair(root, 0));
    p.insert(std::make_pair(root, -1));

    while (!actives.empty()) {
        auto w = actives.begin()->first;
        auto x = actives.begin()->second;
        // remove each term we process to
        // see if this is a complete solution
        term.erase(x);
        for (const auto& [a, e] : ins.nodes[x].edges) {
            // node is saturated and not yet processed
            if (e.capacity == 0 && done.count(a) == 0) {
                // update distance if needed
                if (dist.count(a) == 0) {
                    dist.insert(std::make_pair(a, w+e.weight));
                    actives.insert(std::make_pair(w+e.weight, a));
                    p.insert(std::make_pair(a, x));
                } else if (dist[a] > w+e.weight) {
                    actives.erase(std::make_pair(dist[a], a));
                    dist[a] = w+e.weight;
                    actives.insert(std::make_pair(w+e.weight, a));
                    p[a] = x;
                }
            }
        }
        actives.erase(actives.begin());
        done.insert(x);
    }
    // actually build solution from min. dist parent tree
    std::vector<std::pair<size_t,size_t>> edges;
    auto sum = dfs(root, p, edges);
    if (term.empty() && sum < min) {
        min = sum;
        min_edges = edges;
    }
}
