#define PSEUDO_EOF 256
#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <unordered_map>
#include <queue>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cctype>
#include <cmath>

using namespace std;

struct Node {
    int currVal;
    int currChar;
    Node *left;
    Node *right;

    Node(int val){
        currVal = val;
        currChar = '\0';
        left = NULL;
        right = NULL;
    }
};

struct cmp{
    bool operator() (const Node *first, const Node *second){
        return first->currVal > second->currVal;
    }
};

void dfs(Node* node, string code, unordered_map<int, string> &codes);
void dfs(Node* node, string code, unordered_map<string, int> &decodes);
string encode(string source, string dest);
string decode(string source, string dest);
unordered_map<string, int> decodeMap(string codes);
string performDecode(unordered_map<string, int> decodes, string code);
Node* constructTree(priority_queue<Node*, vector<Node*>, cmp> &heap);
void createFile(string encoded, string encoding);
void createDecodedFile(string content);