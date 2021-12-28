#include "encode.h"

// performs depth first search on the huffman tree to generate codes for each character
void dfs(Node* node, string code, unordered_map<int, string> &codes){
    if(node->currChar != '\0'){
        codes[node->currChar] = code;
        return;
    }
    if(node == NULL){
        return;
    }
    dfs(node->left, code + '0', codes);
    dfs(node->right, code + '1', codes);
}

// construct the huffman tree with a priority queue/heap
Node* constructTree(priority_queue<Node*, vector<Node*>, cmp> &heap){
    Node* top;
    while(!heap.empty()){
        Node* currJoin = new Node(0);
        Node* leftNode = heap.top();
        heap.pop();
        Node* rightNode = !heap.empty() ? heap.top() : NULL;
        heap.pop();
        currJoin->left = leftNode;
        currJoin->right = rightNode;
        currJoin->currVal += leftNode->currVal;
        if(rightNode != NULL)
            currJoin->currVal += rightNode->currVal;
        if(!heap.empty())
            heap.push(currJoin);
        top = currJoin;
    }
    return top;
}

// creates parsable file parsable file that has enough info to rebuild huffman tree + the compressed binary code
void createFile(string encoded, string code, string filename){
    ofstream encodedFile(filename);
    encodedFile << encoded;

    // data has to be appened to a file a byte at a time
    u_int8_t curr = 0;
    int bitshift = 0;
    for(int i = 0; i < code.size(); i++){
        curr <<= 1;
        if(code[i] == '1'){
            curr |= 1;
        }
        bitshift++;
        // when the byte has been fully repopulated, append to file
        if(bitshift == 8){
            encodedFile << curr;
            bitshift = 0;
        }
    }
    // get the last remaining byte if it hasn't been appended to the file
    if(bitshift != 8){
        while(bitshift < 8){
            curr <<= 1;
            bitshift++;
        }
        encodedFile << curr;
    }
    
    encodedFile.close();
}

string encode(string source, string dest){
    ifstream fileToEncode(source);
    if(!fileToEncode.is_open()){
        cout << source << " does not exist" << endl;
        exit(1);
    }
    stringstream strStream;
    strStream << fileToEncode.rdbuf();
    string toCompress = strStream.str();
    priority_queue<Node*, vector<Node*>, cmp> heap;
    unordered_map<int, int> charCounts;

    for(char ch : toCompress){
        charCounts[ch]++;
    }
    unordered_map<int, int>:: iterator it;

    // create heap based on the number of occurances of a character
    for (it = charCounts.begin(); it != charCounts.end(); it++){
        Node* curr = new Node(it->second);
        curr->currVal = it->second;
        curr->currChar = it->first;
        heap.push(curr);
    }

    // fake eof node added to heap
    Node* eof = new Node(1);
    eof->currChar = PSEUDO_EOF;
    heap.push(eof);
    charCounts[PSEUDO_EOF] = 1;

    // tree constructed and huffman codes are generated with dfs
    Node* top = constructTree(heap);
    unordered_map<int, string> codes;
    dfs(top, "", codes);

    // parsable string is created
    string encoded = to_string(charCounts.size() - 1);
    string first = "";
    string second = "";
    for (auto pair : charCounts){
        if(pair.first == PSEUDO_EOF)
            continue;

        first += pair.first;
        second += to_string(pair.second);
        second += ",";
    }
    encoded += first;
    encoded += second;
    string code = "";
    for(char ch : toCompress){
        code += codes[ch];
    }
    // fake eof code is added to denote when to stop reading file to prevent reading garbage bits when decompressing
    code += codes[PSEUDO_EOF];
    // file is created
    createFile(encoded, code, dest);
    encoded += code;
    return encoded;
}   