#include "encode.h"
using namespace std;
// performs depth first search on the huffman tree to generate codes for each
// character
void dfs(unique_ptr<Node> const& node, string const& code, unordered_map<string, int>& decodes)
{
    if (node->currChar != '\0') {
        decodes[code] = node->currChar;
        return;
    }
    if (node == nullptr) {
        return;
    }
    dfs(node->left, code + '0', decodes);
    dfs(node->right, code + '1', decodes);
}

// parses the frequency mapping to create the huffman tree and eventually
// traverse it
unordered_map<string, int> decodeMap(string codes, int* r)
{
    priority_queue<std::unique_ptr<Node>, vector<std::unique_ptr<Node>>, cmp> heap;
    unordered_map<string, int> decodes;
    int index = 0;
    string strNumChars;
    while (isdigit(codes[index])) {
        strNumChars += codes[index];
        index++;
    }

    // get the value of how many characters there are to scan for
    int numChars = stoi(strNumChars);

    int j = index + numChars;

    // set up heap relating to the number of occurrences for a particular character
    for (int i = index; i < numChars + index; i++) {
        int comma = codes.find(',', j);
        int key = codes[i];
        int val = stoi(codes.substr(j, comma));
        auto curr = std::make_unique<Node>(val);
        curr->currChar = key;
        heap.push(std::move(curr));
        j = comma + 1;
    }

    // fake EOF node to make sure garage values dont get decoded
    auto eof = std::make_unique<Node>(1);
    eof->currChar = PSEUDO_EOF;
    heap.push(std::move(eof));

    auto top = constructTree(heap);
    dfs(top, "", decodes);
    *r = j;
    return decodes;
}

// turns the compressed binary code into the original content
string performDecode(unordered_map<string, int> decodes, string bitsEncoded)
{
    string encoded = "";
    // contract each bit from each byte from the compressed binary in the file
    for (int i = 0; i < bitsEncoded.size(); i++) {
        int j = 0;
        string curr = "";
        uint8_t currByte = (uint8_t) bitsEncoded[i];
        // keep extracting the most significant bit, then second most, then third,
        // etc
        while (j < 8) {
            curr += ((currByte & 0x80)) ? '1' : '0';
            currByte <<= 1;
            j++;
        }
        encoded += curr;
    }

    int l, r = 0;
    string resultant;
    string temp;
    // build the string until we hit the PSEUDO_EOF
    while (r < encoded.size()) {
        temp += encoded[r];
        if (decodes.count(temp)) {
            if (decodes[temp] == PSEUDO_EOF) {
                break;
            }

            resultant += decodes[temp];
            temp = "";
            l = r + 1;
        }
        r += 1;
    }
    return resultant;
}

void createDecodedFile(string const& decompressedContent, string const& dest)
{
    ofstream decodedFile(dest);
    decodedFile << decompressedContent;
}

string decode(string const& source, string const& dest)
{
    ifstream encodedFile(source);
    if (!encodedFile.is_open()) {
        cout << source << " does not exist" << endl;
        exit(1);
    }

    string toDecode;
    std::copy(std::istreambuf_iterator<char>{encodedFile},
              std::istreambuf_iterator<char>{},
              std::back_inserter(toDecode));
    // this value is used to separate the character frequency mappings from the
    // compressed binary
    int split;
    // its found using this function where the pointer is passed in
    unordered_map<string, int> decodes = decodeMap(toDecode, &split);
    string bitsEncoded = toDecode.substr(split, toDecode.size() - 1);
    string decompressedContent = performDecode(decodes, bitsEncoded);

    createDecodedFile(decompressedContent, dest);
    return decompressedContent;
}