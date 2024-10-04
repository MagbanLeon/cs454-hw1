#include <fstream>
#include <iostream>
#include <string>
using namespace std;

//g++ -o des DES.cpp

int main(int argc, char* argv[]){

    //Check if command is correctly written/has necessaary arguments.
    if(argc != 3){
        cout << "There aren't enough parameters!" << endl << "Enter as './des sample_input.txt output.txt'" << endl;
        return 1;
    }

    //Input file
    ifstream readingFile(argv[1]);
    FILE* readFile;
    readFile = fopen(argv[1], "r");

    //Error
    if (!readingFile.is_open()) {
        cout << "Error opening the file!" << endl;
        return 1;
    }

    string data_blockLine, keyLine, operationLine, opType;
    unsigned int data, key;
    cout << "File Content: " << endl;
    while (cin(readFile, "%9s %X \n %9s %X \n %9s %9s", data_blockLine, &data, keyLine, &key, operationLine, opType) != EOF) {
        cout << data_blockLine << endl << data << endl << keyLine  << endl << key << endl << operationLine << endl << opType << endl;
    }

    readingFile.close();
    return 0;
}