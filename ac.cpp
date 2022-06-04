#include <iostream>
#include <queue>

#include"ac.h"

AC::AC() {
    Node root;
    bohr.push_back(root);
}

void AC::addPattern(const std::string& pattern) {
    this->patterns.push_back(pattern);
    int curr_node = 0;
    for (const auto symb : pattern) {
        if (this->bohr[curr_node].next_nodes[128 + int(symb)] == -1) {
            Node newNode;
            newNode.symbol = symb;
            newNode.parent = curr_node;
            this->bohr[curr_node].next_nodes[128 + int(symb)] = int(this->bohr.size());
            curr_node = this->bohr[curr_node].next_nodes[128 + int(symb)];
            this->bohr.push_back(newNode);
        }
        else {
            curr_node = this->bohr[curr_node].next_nodes[128 + int(symb)];
        }
    }
    this->bohr[curr_node].pattern_index = this->patterns.size() - 1;
    this->bohr[curr_node].leaf = true;
}

int AC::get_suff_link(const int node, const char symb) {
    if (node == 0) {
        return 0;
    }
    else {
        auto next_node = go_next_node(this->bohr[node].suff_link, symb);
        if (next_node != -1) {
            return next_node;
        }
        else {
            return get_suff_link(this->bohr[node].suff_link, symb);
        }
    }
}

void AC::make_suff_links() {
    std::queue<int> q;
    q.push(0);
    while (!q.empty()) {
        auto& curr_node = this->bohr[q.front()];
        curr_node.suff_link = get_suff_link(curr_node.parent, curr_node.symbol);
        std::for_each(curr_node.next_nodes.begin(), curr_node.next_nodes.end(),
            [&q](const auto node) { if (node != -1) q.push(node); });
        q.pop();
    }
}

void AC::init(const std::vector<std::string> patterns) {
    for (const auto& pattern : patterns) {
        this->addPattern(pattern);
    }
    this->make_suff_links();
}

int AC::go_next_node(const int node, const char symb) {
    int node_child = this->bohr[node].next_nodes[128 + int(symb)];
    if (node_child != -1) {
        return node_child;
    }
    else {
        return -1;
    }
}

int AC::go(const int node, const char symb) {
    auto next_node = go_next_node(node, symb);
    if (next_node != -1) {
        return next_node;
    }
    if (node == 0) {
        return 0;
    }

    return go(this->bohr[node].suff_link, symb);
}

void AC::check(const int node, const int ind) {
    for (auto curr_node = node; curr_node != 0; curr_node = this->bohr[curr_node].suff_link) {
        if (this->bohr[curr_node].leaf) {
            std::cout << ind - this->patterns[this->bohr[curr_node].pattern_index].size()
                << this->patterns[this->bohr[curr_node].pattern_index] << std::endl;
        }
    }
}

void AC::find(const std::string& request) {
    int curr_node = 0;
    for (size_t i = 0; i < request.size(); i++) {
        curr_node = go(curr_node, request[i]);
        check(curr_node, i + 1);
    }
}

void AC::find_all_patterns_inclusions_in_string(std::string patterns, const std::string string) {
    std::string delimiter = "; ";
    size_t ind = 0;
    std::string pattern;
    std::vector<std::string> patterns_vec;
    while ((ind = patterns.find(delimiter)) != std::string::npos) {
        pattern = patterns.substr(0, ind);
        patterns_vec.push_back(pattern);
        patterns.erase(0, ind + delimiter.length());
    }
    patterns_vec.push_back(patterns);
    this->init(patterns_vec);
    this->find(string);
}
