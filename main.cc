#include "encode.h"

int main(){
    string toCompress = "big bob bites bananas";
    string coded = encode(toCompress);
    cout << coded << endl;
    cout << decode(coded) << endl;

    return 0;
}