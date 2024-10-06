#include <fstream>
#include <iostream>
#include <string>
#include <iomanip>
using namespace std;

//g++ -o des DES.cpp
//./des sample_input_encryption.txt output.txt

//Function Declarations
unsigned long int decryption();
unsigned long int encryption();
void keys(unsigned long int initialKey);

//Main Function
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

    int n, k, bit;

    //GET the k-th bit of n
    bit  = (n & ( 1 << k )) >> k;

    //SET the k-th bit of n
    n = n | 1 << k;

    //CLEAR the k-th bit of n
    n = n & ~ (1 << k);

    //arrays
    unsigned long int cKey[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    unsigned long int dKey[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    unsigned long int keyEntire[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    unsigned long int left[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    unsigned long int right[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    
    fclose(readFile);
    return 0;
}

//Function Definitions
unsigned long int decryption(){

}

unsigned long int encryption(){

}

void keys(unsigned long int initialKey){
    int pc1[56] = {57,49,41,33,25,17,9,1,58,50,42,34,26,18,10,2,59,51,43,35,27,19,11,3,60,52,
    44,36,63,55,47,39,31,23,15,7,62,54,46,38,30,22,14,6,61,53,45,37,29,21,13,5,28,20,12,4};
    unsigned long int keyPlus = 0;
    int bit, n, k;

    //getting keyPlus
    for(int i = 0; i < 56; i++){
        bit  = (initialKey & ( 1 << pc1[i] )) >> pc1[i];    //get bit from initial key
        if(bit == 1){
            keyPlus = keyPlus | 1 << i;     //set bit i if bit i in intital key is 1
        }else{
            keyPlus = keyPlus & ~ (1 << i); //clear bit i if bit i in intital key is 0
        }
    }

    //getting C0 and D0
}   