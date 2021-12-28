#include <algorithm>
#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <memory>
#include <queue>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

constexpr int PSEUDO_EOF = 256;

struct Node {
    int currVal;
    int currChar;
    std::unique_ptr<Node> left;
    std::unique_ptr<Node> right;

    Node(int val)
    {
        currVal = val;
        currChar = '\0';
    }

    friend bool operator>(Node const& lhs, Node const& rhs);
};

struct cmp {
    bool operator()(std::unique_ptr<Node>& first,
                    std::unique_ptr<Node> const& second)
    {
        return first->currVal > second->currVal;
    }
};

void dfs(std::unique_ptr<Node> const& node, std::string const& code, std::unordered_map<int, std::string>& codes);

void dfs(std::unique_ptr<Node> const& node, const std::string& code, std::unordered_map<std::string, int>& decodes);

std::string encode(std::string const& source, std::string const& dest);

std::string decode(const std::string& source, const std::string& dest);

std::unordered_map<std::string, int> decodeMap(std::string codes);

std::string performDecode(std::unordered_map<std::string, int> decodes, std::string code);

std::unique_ptr<Node> constructTree(std::priority_queue<std::unique_ptr<Node>, std::vector<std::unique_ptr<Node>>, cmp>& heap);

void createFile(std::string encoded, std::string encoding);

void createDecodedFile(std::string content);