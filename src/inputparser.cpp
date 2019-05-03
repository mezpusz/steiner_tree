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
            is >> ins.nodes;
        } else if (first == "E") {
            size_t u, v, w;
            is >> u >> v >> w;
            ins.edges.insert(std::make_pair(std::make_pair(u, v), w));
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
            ins.terminals.push_back(u);
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
