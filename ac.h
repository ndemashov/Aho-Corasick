#pragma once

#include <vector>
#include <string>
#include <algorithm>

class AC {
    struct Node {
        bool leaf = false;
        char symbol = '\0';
        int parent = 0;
        int pattern_index = 0;
        int suff_link = 0;
        std::vector<int> next_nodes = std::vector<int>(255, -1);

    };
    std::vector<Node> bohr;
    std::vector<std::string> patterns;

public:
    AC();
    void init(const std::vector<std::string> patterns);
    void addPattern(const std::string& pattern);
    int get_suff_link(const int node, const char symb);
    void make_suff_links();
    int go_next_node(const int node, const char symb);
    int go(const int node, const char symb);
    void check(const int node, const int ind);
    void find(const std::string& request);
    void find_all_patterns_inclusions_in_string(std::string patterns, const std::string string);
};
