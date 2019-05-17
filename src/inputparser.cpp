#include "inputparser.h"

#include <fstream>
#include <iostream>
#include <sstream>

instance parse(const std::string& filename) {
    auto sections = get_sections(filename);
    instance ins;
    // parse graph
    for (const auto& line : sections["Graph"]) {
        std::istringstream is(line);
        std::string first;
        is >> first;
        if (first == "Nodes") {
            size_t nodes_count;
            is >> nodes_count;
            ins.nodes.resize(nodes_count+1); // node ids start from 1
        } else if (first == "A") {
            size_t u, v, weight;
            is >> u >> v >> weight;
            ins.nodes[u].edges.insert(std::make_pair(v, edge(weight)));
            ins.nodes[v].redges.insert(u);
        } else if (first == "E") {
            size_t u, v, weight;
            is >> u >> v >> weight;
            ins.nodes[u].edges.insert(std::make_pair(v, edge(weight)));
            ins.nodes[v].edges.insert(std::make_pair(u, edge(weight)));
            ins.nodes[u].redges.insert(v);
            ins.nodes[v].redges.insert(u);
        }
    }
    // parse terminals
    for (const auto& line : sections["Terminals"]) {
        std::istringstream is(line);
        std::string first;
        is >> first;
        if (first == "T") {
            size_t u;
            is >> u;
            terminal t(u, ins.nodes[u].redges.size());
            ins.terminals.insert(std::make_pair(u, t));
        }
    }
    return ins;
}

std::map<std::string, std::vector<std::string>> get_sections(const std::string& filename) {
    std::ifstream in(filename);
    std::map<std::string, std::vector<std::string>> res;
    if (!in.is_open()) {
        std::cout << "Cannot open input file " << filename << std::endl;
        return res;
    }

    std::string line;
    std::string name;
    std::vector<std::string> sv;
    bool section = false;
    while (std::getline(in, line)) {
        if (line.empty() || line[0] == '#') {
            continue;
        }
        if (section) {
            if (line.find("END") == 0) {
                section = false;
                res.insert(make_pair(name, sv));
            } else {
                sv.push_back(line);
            }
        } else if (line.find("SECTION") == 0) {
            section = true;
            name = line.substr(line.find(' ')+1);
        }
    }
    return res;
}
