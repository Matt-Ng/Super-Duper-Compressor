#include "encode.h"

int main(int argc, char **argv){
    if (argc == 1){
        cout << "press 1 if you wanna compress a file press 2 if you wanna decompress" << endl;
        string option;
        cin >> option;
        if (option == "1"){
            cout << "enter the file that you wanna compress" << endl;
            string source;
            cin >> source;
            cout << "enter the filename you want to your compressed data to use" << endl;
            string dest;
            cin >> dest;
            encode(source, dest);
        }
        else if (option == "2"){
            cout << "enter the file that you wanna decompress" << endl;
            string source;
            cin >> source;
            cout << "enter the filename you want to your decompressed data to use" << endl;
            string dest;
            cin >> dest;
            decode(source, dest);
        }
        else{
            cout << "professor oak's voice echoed: theres 2 options, 1 or 2 " << option << " is not an option!!!" << endl;
            return 1;
        }
    }
    else if(argc == 4){
        string flag = argv[1];
        if (flag == "-c" || flag == "-compress"){
            encode(argv[2], argv[3]);
        }
        else if(flag == "-d" || flag == "-decompress"){
            decode(argv[2], argv[3]);
        }
        else{
            cout << "usage: ./huffman -[compress|c|d|decompress] sourceFilename destFilename" << endl;
            return 1;
        }
    }
    else{
        cout << "either you can just do ./huffman or you can do ./huffman -[compress|c|d|decompress] sourceFilename destFilename" << endl;
        return 1;
    }

    cout << "success, congrats, bye" << endl;
    return 0;
}