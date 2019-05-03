
#include "inputparser.h"

#include <iostream>
#include <fstream>

int main(int argc, char* argv[]) {
    std::srand(0);
    std::string infile_str, outfile_str;
    infile_str = "B/b01.stp";
    outfile_str = "output.txt";
    auto instance = parse(infile_str);
    std::cout << "Found " << instance.nodes << " nodes, "
              << instance.edges.size() << " edges and "
              << instance.terminals.size() << " terminals in input file" << std::endl;

    std::ofstream out(outfile_str);
    if (out.is_open()) {
        out.close();
    } else {
        return -1;
    }

    return 0;
}