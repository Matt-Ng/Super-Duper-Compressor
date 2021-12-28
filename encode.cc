#include "encode.h"

using namespace std;

// performs depth first search on the huffman tree to generate codes for each
// character
void dfs(unique_ptr<Node> const& node, string const& code,
         unordered_map<int, string>& codes)
{
    if (node->currChar != '\0') {
        codes[node->currChar] = code;
        return;
    }
    if (node == nullptr) {
        return;
    }
    dfs(node->left, code + '0', codes);
    dfs(node->right, code + '1', codes);
}

// construct the huffman tree with a priority queue/heap
std::unique_ptr<Node> constructTree(priority_queue<unique_ptr<Node>, vector<std::unique_ptr<Node>>, cmp>& heap)
{
    // since an unique pointer can only be held by one instance at a given time, we don't have the luxury to hold the
    // pointer to root after the heap is gone. Instead, we return directly when we have no more nodes to join
    while (true) {
        auto currJoin = std::make_unique<Node>(0);

        // this is a defect of the standard
        auto leftNode = std::move(const_cast<std::unique_ptr<Node>&>(heap.top()));
        heap.pop();

        auto rightNode =
                !heap.empty()
                ? std::move(const_cast<std::unique_ptr<Node>&>(heap.top()))
                : nullptr;
        heap.pop();

        currJoin->left = std::move(leftNode);
        currJoin->right = std::move(rightNode);
        currJoin->currVal += currJoin->left->currVal;

        if (rightNode != nullptr)
            currJoin->currVal += currJoin->right->currVal;

        if (!heap.empty())
            heap.push(std::move(currJoin));
        else
            return currJoin;
    }
}

// creates parsable file that has enough info to rebuild huffman
// tree + the compressed binary code
void createFile(string const& encoded, string const& code, string const& filename)
{
    ofstream encodedFile(filename);
    encodedFile << encoded;

    // data has to be appended to a file a byte at a time
    u_int8_t curr = 0;
    int bitshift = 0;
    for (char i: code) {
        curr <<= 1;
        if (i == '1') {
            curr |= 1;
        }
        bitshift++;
        // when the byte has been fully repopulated, append to file
        if (bitshift == 8) {
            encodedFile << curr;
            bitshift = 0;
        }
    }
    // get the last remaining byte if it hasn't been appended to the file
    while (bitshift < 8) {
        curr <<= 1;
        bitshift++;
    }
    encodedFile << curr;

    encodedFile.close();
}

string encode(string const& source, string const& dest)
{
    ifstream fileToEncode(source);
    if (!fileToEncode.is_open()) {
        cout << source << " does not exist" << endl;
        exit(1);
    }

    string toCompress;
    std::copy(std::istreambuf_iterator<char>{fileToEncode},
              std::istreambuf_iterator<char>{},
              std::back_inserter(toCompress));

    priority_queue<std::unique_ptr<Node>, vector<std::unique_ptr<Node>>, cmp> heap;
    unordered_map<int, int> charCounts;

    for (char ch: toCompress) {
        charCounts[ch]++;
    }

    // create heap based on the number of occurances of a character
    for (auto pair: charCounts) {
        auto curr = std::make_unique<Node>(pair.second);
        curr->currVal = pair.second;
        curr->currChar = pair.first;
        heap.push(std::move(curr));
    }

    // fake eof node added to heap
    auto eof = std::make_unique<Node>(1);
    eof->currChar = PSEUDO_EOF;
    heap.push(std::move(eof));
    charCounts[PSEUDO_EOF] = 1;

    // tree constructed and huffman codes are generated with dfs
    auto top = constructTree(heap);
    unordered_map<int, string> codes;
    dfs(top, "", codes);

    // parsable string is created
    string encoded = to_string(charCounts.size() - 1);
    string first;
    string second;

    for (auto pair: charCounts) {
        if (pair.first == PSEUDO_EOF)
            continue;

        first += pair.first;
        second += to_string(pair.second);
        second += ",";
    }
    encoded += first;
    encoded += second;
    string code;

    for (char ch: toCompress) {
        code += codes[ch];
    }

    // fake eof code is added to denote when to stop reading file to prevent
    // reading garbage bits when decompressing
    code += codes[PSEUDO_EOF];
    // file is created
    createFile(encoded, code, dest);
    encoded += code;
    return encoded;
}
bool operator>(Node const& lhs, Node const& rhs)
{
    return lhs.currVal > rhs.currVal;
}
