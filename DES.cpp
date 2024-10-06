#include <fstream>
#include <iostream>
#include <string>
#include <iomanip>
using namespace std;

//g++ -o des DES.cpp
//./des sample_input_encryption.txt output.txt

//Function Declarations
unsigned long int decryption();
unsigned long int encryption(unsigned long int keyArray[16], unsigned long int message, unsigned long int lSide[17], unsigned long int rSide[17]);
void keys(unsigned long int initialKey, unsigned long int cArray[17], unsigned long int dArray[17], unsigned long int keyArray[16]);
void lefRightIterations(unsigned long int left[17], unsigned long int right[17], unsigned long int keys[16]);
unsigned long int functionF(unsigned long int right, unsigned long int key);

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
    cout << setfill('0') << setw(16) << uppercase << hex << uintptr_t(data) << endl << uppercase << hex << uintptr_t(key) << endl << operation << endl;
    
    //Choosing which process
    if(operation == "decryption"){

    }else if(operation == "encryption"){

    }else{
        cout << "Error reading file." << endl;
    }

    int n, k, bit;

    //GET the k-th bit of n
    bit = (n & ( 1 << k )) >> k;

    //SET the k-th bit of n
    n = n | k << k;

    //CLEAR the k-th bit of n
    n = n & ~ (1 << k);

    //arrays
    unsigned long int cKey[17] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    unsigned long int dKey[17] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    unsigned long int keyEntire[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    unsigned long int left[17] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    unsigned long int right[17] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

    keys(key, cKey, dKey, keyEntire);


    
    fclose(readFile);
    return 0;
}

//Function Definitions
unsigned long int decryption(){

}

unsigned long int encryption(unsigned long int keyArray[16], unsigned long int message, unsigned long int lSide[17], unsigned long int rSide[17]){
    int ipTable[64] = {58, 50, 42, 34, 26, 18, 10, 2,
				    60, 52, 44, 36, 28, 20, 12, 4,
                    62, 54, 46, 38, 30, 22, 14, 6,
                    64, 56, 48, 40, 32, 24, 16, 8,
                    57, 49, 41, 33, 25, 17,  9, 1,
                    59, 51, 43, 35, 27, 19, 11, 3,
                    61, 53, 45, 37, 29, 21, 13, 5,
                    63, 55, 47, 39, 31, 23, 15, 7};
    int bit = 0;
    unsigned long int ip = 0;

    //Initial permutation
    for(int i = 0; i < 64; i++){
        bit = (message & ( 1 << (ipTable[i]-1) )) >> (ipTable[i]-1);    //get bit from initial key
        if(bit == 1){
            ip = ip | 1 << i;     //set bit i if bit i in intital key is 1
        }
    }

    //Getting L0 and R0
    //L0, take bits 32-63 and put them into 0-32
    for(int i = 0; i < 32; i++){
        bit = (ip & ( 1 << (i+32) )) >> (i+32);
        if(bit == 1){
            lSide[0] = lSide[0] | i << i;
        }
    }

    //R0, clear bits 32-63
    rSide[0] = ip;
    for(int i = 32; i < 64; i++){
        bit = (rSide[0] & ( 1 << i )) >> i;
        if(bit == 1){
            rSide[0] = rSide[0] & ~ (1 << i);
        }
    }
    lefRightIterations(lSide, rSide, keyArray);


}
    //16 iterations
void lefRightIterations(unsigned long int left[17], unsigned long int right[17], unsigned long int keys[16]){
    for(int i = 1; i < 17; i++){
        left[i] = right[i-1];
        right[i] = left[i-1] ^ functionF(right[i-1], keys[i]);
    }
}
    //Function F(Rn-1, Kn)
unsigned long int functionF(unsigned long int right, unsigned long int key){
    int eTable[] =  {32,1,2,3,4,5,
					4,5,6,7,8,9,
					8,9,10,11,12,13,
                    12,13,14,15,16,17,
                    16,17,18,19,20,21,
                    20,21,22,23,24,25,
                    24,25,26,27,28,29,
                    28,29,30,31,32,1};
}

    //Used for key generation
void keys(unsigned long int initialKey, unsigned long int cArray[17], unsigned long int dArray[17], unsigned long int keyArray[16]){
    int pc1[56] = {57,49,41,33,25,17,9,1,58,50,42,34,26,18,10,2,59,51,43,35,27,19,11,3,60,52,
    44,36,63,55,47,39,31,23,15,7,62,54,46,38,30,22,14,6,61,53,45,37,29,21,13,5,28,20,12,4};

    unsigned long int keyPlus = 0;
    int bit, n, k;

    //getting keyPlus
    for(int i = 0; i < 56; i++){
        bit = (initialKey & ( 1 << (pc1[i]-1) )) >> (pc1[i]-1);    //get bit from initial key
        if(bit == 1){
            keyPlus = keyPlus | 1 << i;     //set bit i if bit i in intital key is 1
        }else{
            keyPlus = keyPlus & ~ (1 << i); //clear bit i if bit i in intital key is 0
        }
    }

    unsigned long int cZ = 0, dZ;
    //getting C0 and D0
    //c0, take bits 28-56 and put them into 0-27
    for(int i = 0; i < 28; i++){
        bit = (keyPlus & ( 1 << (i+28) )) >> (i+28);
        if(bit == 1){
            cZ = cZ | i << i;
        }
    }

    //d0, clear bits 28-55
    dZ = keyPlus;
    for(int i = 28; i < 56; i++){
        bit = (dZ & ( 1 << i )) >> i;
        if(bit == 1){
            dZ = dZ & ~ (1 << i);
        }
    }

    int leftShiftSched[16] = {1,1,2,2,2,2,2,2,1,2,2,2,2,2,2,1};
    //getting Cn and Dn
    cArray[0] = cZ;
    dArray[0] = dZ;
    for(int i = 1; i < 17; i++){       
        cArray[i] = cArray[i-1] << leftShiftSched[i];
        dArray[i] = dArray[i-1] << leftShiftSched[i];
    }

    //getting Kn
    int pc2[48] = {14,17,11,24,1,5,3,28,15,6,21,10,23,19,12,4,26,8,16,7,27,20,13,
    2,41,52,31,37,47,55,30,40,51,45,33,48,44,49,39,56,34,53,46,42,50,36,29,32};

    for(int i = 0; i < 16; i++){
        //Dn
        keyArray[i] = dArray[i+1];

        //Cn
        for(int j = 28; j < 56; j++){
            bit = (cArray[j+1] & ( 1 << j )) >> j;
            if(bit == 1){
                keyArray[i] = keyArray[i] | j << j;
            }
        }

        //put through PC2
        for(int i = 0; i < 48; i++){
            bit = (keyArray[i] & ( 1 << (pc2[i]-1) )) >> (pc2[i]-1);    //get bit from Kn
            if(bit == 1){
                keyArray[i] = keyArray[i] | 1 << i;     //set bit i if bit i in Kn is 1
            }else{
                keyArray[i] = keyArray[i] & ~ (1 << i); //clear bit i if bit i in Kn is 0
            }
        }
    }
}   