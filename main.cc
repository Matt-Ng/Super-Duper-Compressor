#include "encode.h"

int main(){
    string toCompress = "big bob bites bananas";
    string coded = encode(toCompress);
    cout << endl;
    cout << decode(coded) << endl;

    return 0;
}