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
    int eTable[] =  {32,1,2,3,4,5,4,5,6,7,8,9,8,9,10,11,12,13,12,13,14,15,16,17,
                    16,17,18,19,20,21,20,21,22,23,24,25,24,25,26,27,28,29,28,29,30,31,32,1};
    unsigned long int eR = 0, eRxorK = 0, beforeP = 0, finalResult = 0;
    int bit = 0;

    eR = right;

    //E(Rn-1)
    for(int i = 0; i < 56; i++){
        bit = (right & ( 1 << (eTable[i]-1) )) >> (eTable[i]-1);    //get bit from right
        if(bit == 1){
            eR = eR | 1 << i;     //set bit i if bit i in right is 1
        }
    }

    //E(Rn-1) XOR Kn
    eRxorK = eR ^ key;

    //Put through S tables
    int s1[4][16] = {{14,  4, 13,  1,  2, 15, 11,  8,  3, 10,  6, 12,  5,  9,  0,  7},
			        {0, 15,  7,  4, 14,  2, 13,  1, 10,  6, 12, 11,  9,  5,  3,  8},
			        {4,  1, 14,  8, 13,  6,  2, 11, 15, 12,  9,  7,  3, 10,  5,  0},
			        {15, 12,  8,  2,  4,  9,  1,  7,  5, 11,  3, 14, 10,  0,  6, 13}};

    int s2[4][16] = {{15,  1,  8, 14,  6, 11,  3,  4,  9,  7,  2, 13, 12,  0,  5, 10},
                    {3, 13,  4,  7, 15,  2,  8, 14, 12,  0,  1, 10,  6,  9, 11,  5},
                    {0, 14,  7, 11, 10,  4, 13,  1,  5,  8, 12,  6,  9,  3,  2, 15},
                    {13,  8, 10,  1,  3, 15,  4,  2, 11,  6,  7, 12,  0,  5, 14,  9}};

    int s3[4][16] = {{10,  0,  9, 14,  6,  3, 15,  5,  1, 13, 12,  7, 11,  4,  2,  8},
                    {13,  7,  0,  9,  3,  4,  6, 10,  2,  8,  5, 14, 12, 11, 15,  1},
                    {13,  6,  4,  9,  8, 15,  3,  0, 11,  1,  2, 12,  5, 10, 14,  7},
                    {1, 10, 13,  0,  6,  9,  8,  7,  4, 15, 14,  3, 11,  5,  2, 12}};

    int s4[4][16] = {{ 7, 13, 14,  3,  0,  6,  9, 10,  1,  2,  8,  5, 11, 12,  4, 15},
                    {13,  8, 11,  5,  6, 15,  0,  3,  4,  7,  2, 12,  1, 10, 14,  9},
                    {10,  6,  9,  0, 12, 11,  7, 13, 15,  1,  3, 14,  5,  2,  8,  4},
                    {3, 15,  0,  6, 10,  1, 13,  8,  9,  4,  5, 11, 12,  7,  2, 14}};

    int s5[4][16] = {{ 2, 12,  4,  1,  7, 10, 11,  6,  8,  5,  3, 15, 13,  0, 14,  9},
                    {14, 11,  2, 12,  4,  7, 13,  1,  5,  0, 15, 10,  3,  9,  8,  6},
                    {4,  2,  1, 11, 10, 13,  7,  8, 15,  9, 12,  5,  6,  3,  0, 14},
                    {11,  8, 12,  7,  1, 14,  2, 13,  6, 15,  0,  9, 10,  4,  5,  3}};

    int s6[4][16] = {{12,  1, 10, 15,  9,  2,  6,  8,  0, 13,  3,  4, 14,  7,  5, 11},
                    {10, 15,  4,  2,  7, 12,  9,  5,  6,  1, 13, 14,  0, 11,  3,  8},
                    {9, 14, 15,  5,  2,  8, 12,  3,  7,  0,  4, 10,  1, 13, 11,  6},
                    {4,  3,  2, 12,  9,  5, 15, 10, 11, 14,  1,  7,  6,  0,  8, 13}};

    int s7[4][16] = {{ 4, 11,  2, 14, 15,  0,  8, 13,  3, 12,  9,  7,  5, 10,  6,  1},
                    {13,  0, 11,  7,  4,  9,  1, 10, 14,  3,  5, 12,  2, 15,  8,  6},
                    {1,  4, 11, 13, 12,  3,  7, 14, 10, 15,  6,  8,  0,  5,  9,  2},
                    {6, 11, 13,  8,  1,  4, 10,  7,  9,  5,  0, 15, 14,  2,  3, 12}};

    int s8[4][16] = {{13,  2,  8,  4,  6, 15, 11,  1, 10,  9,  3, 14,  5,  0, 12,  7},
                    {1, 15, 13,  8, 10,  3,  7,  4, 12,  5,  6, 11,  0, 14,  9,  2},
                    {7, 11,  4,  1,  9, 12, 14,  2,  0,  6, 10, 13, 15,  3,  5,  8},
                    {2,  1, 14,  7,  4, 10,  8, 13, 15, 12,  9,  0,  3,  5,  6, 11}};

    

    return finalResult;
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