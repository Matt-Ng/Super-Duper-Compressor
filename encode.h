#define PSEUDO_EOF 256
#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <unordered_map>
#include <queue>
#include <ctype.h>
#include <math.h>

using namespace std;

struct Node {
    int currVal;
    int currChar;
    struct Node *left;
    struct Node *right;

    Node(int val){
        currVal = val;
        currChar = '\0';
        left = NULL;
        right = NULL;
    }
};

struct cmp{
    bool operator() (const struct Node *first, const struct Node *second){
        return first->currVal > second->currVal;
    }
};

void dfs(struct Node* node, string code, unordered_map<int, string> &codes);
void dfs(struct Node* node, string code, unordered_map<string, int> &decodes);
string encode(string toCompress);
string decode(string toDecode);
unordered_map<string, int> decodeMap(string codes);
string performDecode(unordered_map<string, int> decodes, string encoded);
struct Node* constructTree(priority_queue<struct Node*, vector<struct Node*>, cmp> &heap);