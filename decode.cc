#include "encode.h"

void dfs(struct Node* node, string code, unordered_map<string, int> &decodes){
    if(node->currChar != '\0'){
        decodes[code] = node->currChar;
        return;
    }
    if(node == NULL){
        return;
    }
    dfs(node->left, code + '0', decodes);
    dfs(node->right, code + '1', decodes);
}

unordered_map<string, int> decodeMap(string codes, int *r){
    priority_queue<struct Node*, vector<struct Node*>, cmp> heap;
    unordered_map<string, int> decodes;
    int index = 0;
    string strNumChars = "";
    while(isdigit(codes[index])){
        strNumChars += codes[index];
        index++;
    }
    int numChars = stoi(strNumChars);
    
    int j = index + numChars;

    for(int i = index; i < numChars + index; i++){
        int comma = codes.find(",", j);
        int key = codes[i];
        int val = stoi(codes.substr(j, comma));
        struct Node* curr = new Node(val);
        curr->currChar = key;
        heap.push(curr);
        j = comma + 1;
    }
    struct Node* eof = new Node(1);
    eof->currChar = PSEUDO_EOF;
    heap.push(eof);

    struct Node* top = constructTree(heap);
    dfs(top, "", decodes);
    *r = j;
    return decodes;
}

string performDecode(unordered_map<string, int> decodes, string bitsEncoded){
    string encoded = "";
    for(int i = 0; i < bitsEncoded.size(); i++){
        int j = 0;
        string curr = "";
        uint8_t currByte = (uint8_t) bitsEncoded[i];
        cout << "byte" << currByte;
        while(j < 8){
            curr += ((currByte & 0x80)) ? '1' : '0';
            currByte <<= 1;
            j++;
        }
        encoded += curr;
    }
    cout << endl;
    cout << encoded << endl;
    int l, r = 0;
    string resultant = "";
    string temp = "";
    while(r < encoded.size()){
        temp += encoded[r];
        if(decodes.count(temp)){
            if(decodes[temp] == PSEUDO_EOF)
                break;

            resultant += decodes[temp];
            temp = "";
            l = r + 1;
        }
        r += 1;
    }
    return resultant;
}

void createDecodedFile(string content){
    ofstream decodedFile("uncompressed.txt");
    decodedFile << content;
    decodedFile.close();
}

string decode(string toDecode){
    ifstream encodedFile("compress.txt");
    stringstream strStream;
    strStream << encodedFile.rdbuf();
    toDecode = strStream.str();
    int split;
    unordered_map<string, int> decodes = decodeMap(toDecode, &split);
    string bitsEncoded = toDecode.substr(split, toDecode.size() - 1);
    cout << bitsEncoded << endl;
    string decompressedContent = performDecode(decodes, bitsEncoded);
    encodedFile.close();
    createDecodedFile(decompressedContent);
    return decompressedContent;
}