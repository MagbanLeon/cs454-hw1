#include <fstream>
#include <iostream>
#include <string>
#include <iomanip>
using namespace std;

//g++ -o des DES.cpp

int main(int argc, char* argv[]){

    //Check if command is correctly written/has necessaary arguments.
    if(argc != 3){
        cout << "There aren't enough parameters!" << endl << "Enter as './des sample_input.txt output.txt'" << endl;
        return 1;
    }

    //Input file
    FILE* readFile;
    readFile = fopen(argv[1], "r");

    //Error
    if (readFile == NULL) {
        cout << "Error opening the file!" << endl;
        return 1;
    }

    string operation;
    char opType[10];
    unsigned long int data = 0, key = 0;
    
    //Read input file
    fscanf(readFile, "data_block: %lx\nkey: %lx\noperation: %s", &data, &key, opType);
    operation = opType;
    cout << setfill('0') << setw(16) << hex << data << endl << hex << key << endl << operation << endl;
    
    //Choosing which process
    if(operation == "decryption"){
        cout << 1 << endl;

    }else if(operation == "encryption"){
        cout << 2 << endl;

    }else{
        cout << "Error reading file." << endl;
    }

    fclose(readFile);
    return 0;
}