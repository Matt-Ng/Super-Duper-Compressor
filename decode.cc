#include "encode.h"

void dfs(struct Node* node, string code, unordered_map<string, int> &decodes){
    if(node->currChar != '\0'){
        cout << (char) node->currChar << ":" << code << "\n";
        decodes[code] = node->currChar;
        return;
    }
    if(node == NULL){
        return;
    }
    dfs(node->left, code + '0', decodes);
    dfs(node->right, code + '1', decodes);
}

unordered_map<string, int> decodeMap(string codes){
    cout << codes << endl;
    priority_queue<struct Node*, vector<struct Node*>, cmp> heap;
    unordered_map<string, int> decodes;
    int index = 0;
    string strNumChars = "";
    while(isdigit(codes[index])){
        strNumChars += codes[index];
        index++;
    }
    int numChars = stoi(strNumChars);

    cout << index << endl;
    int j = index + numChars;
    cout << "j " << j << endl;
    for(int i = index; i < numChars + index; i++){
        int comma = codes.find(",", j);
        int key = codes[i];
        cout << codes.substr(j, comma) << endl;
        int val = stoi(codes.substr(j, comma));
        cout << (char) key << " " << val << endl;
        struct Node* curr = new Node(val);
        curr->currChar = key;
        heap.push(curr);
        cout << comma << endl;
        j = comma + 1;
    }
    /*
    for(int i = 0; i < codes.size(); i++){
        if(codes[i] != ','){
            temp += codes[i];
        }
        else{
            int comma = temp.find(":");
            int key = temp[0];
            int val = stoi(temp.substr(comma + 1, temp.size() - 1));
            struct Node* curr = new Node(val);
            curr->currChar = key;
            heap.push(curr);
            temp = "";
            //cout << " key " << key << " val " << val << endl;
        }
    }
    int comma = temp.find(":");
    int key = temp[0];
    int val = stoi(temp.substr(comma + 1, temp.size() - 1));
    struct Node* curr = new Node(val);
    curr->currChar = key;
    heap.push(curr);
    */
    struct Node* eof = new Node(1);
    eof->currChar = PSEUDO_EOF;
    heap.push(eof);

    struct Node* top = constructTree(heap);
    dfs(top, "", decodes);
    return decodes;
}

string performDecode(unordered_map<string, int> decodes, string encoded){
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

string decode(string toDecode){
    int split = toDecode.size() - 1;
    while (toDecode[split] != ',' && split != -1){
        split--;
    }
    if(split == -1){
        cout << "Error: invalid compressed file" << endl;
    }
    string codes = toDecode.substr(0, split + 1);
    string encoded = toDecode.substr(split + 1, toDecode.size() - 1);
    unordered_map<string, int> decodes = decodeMap(codes);
    return performDecode(decodes, encoded);
}