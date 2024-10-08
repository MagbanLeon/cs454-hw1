#include <fstream>
#include <iostream>
#include <string>
#include <iomanip>
#include <bitset>
using namespace std;

//g++ -o des DES.cpp
//./des sample_input_encryption.txt output.txt

//Function Declarations
void keys(unsigned long int initialKey, unsigned long int cArray[17], unsigned long int dArray[17], unsigned long int keyArray[16]);


unsigned long int decryption(unsigned long int keyArray[16], unsigned long int encrypted, unsigned long int lSide[17], unsigned long int rSide[17]);
void decryptLeftRight(unsigned long int left[17], unsigned long int right[17], unsigned long int keys[16]);
unsigned long int undoF(unsigned long int finalResult, unsigned long int key);

unsigned long int encryption(unsigned long int keyArray[16], unsigned long int message, unsigned long int lSide[17], unsigned long int rSide[17]);
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
    fclose(readFile);
    //cout << setfill('0') << setw(16) << uppercase << hex << uintptr_t(data) << endl << uppercase << hex << uintptr_t(key) << endl << operation << endl;
    ofstream myfile;
    myfile.open (argv[2]);

    unsigned long int cKey[17] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    unsigned long int dKey[17] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    unsigned long int keyEntire[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    unsigned long int left[17] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    unsigned long int right[17] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

    //Choosing which process
    unsigned long int answer;
    keys(key, cKey, dKey, keyEntire);
    if(operation == "decryption"){
        answer = decryption(keyEntire, data, left, right);
    }else if(operation == "encryption"){
        answer = encryption(keyEntire, data, left, right);
    }else{
        cout << "Error reading file." << endl;
    }
    //Printing to file
    for(int i = 0; i < 17; i++){
        bitset<28> n1 (cKey[i]);
        myfile << "C" << i << "=" << setfill('0') << setw(28) << n1.to_string() << endl;
        bitset<28> n2 (dKey[i]);
        myfile << "D" << i << "=" << setfill('0') << setw(28) << n2.to_string() << endl;
    }
    myfile << endl;
    for(int i = 0; i < 17; i++){
        bitset<48> n3 (keyEntire[i]);
        myfile << "K" << i+1 << "=" << setfill('0') << setw(48) << n3.to_string() << endl;
    }
    myfile << endl;
    for(int i = 0; i < 17; i++){
        bitset<32> n4 (left[i]);
        myfile << "L" << i << "=" << setfill('0') << setw(32) << n4.to_string() << endl;
        bitset<32> n5 (right[i]);
        myfile << "R" << i << "=" << setfill('0') << setw(32) << n5.to_string() << endl;
    }
    myfile << endl << "Result=" << setfill('0') << setw(16) << uppercase << hex << uintptr_t(answer) << endl;
    myfile.close();
    
    return 0;
}

//Function Definitions
unsigned long int decryption(unsigned long int keyArray[16], unsigned long int encrypted, unsigned long int lSide[17], unsigned long int rSide[17]){
    int invPerm[64] = { 40 ,8  ,48 ,16 ,56 ,24 ,64 ,32 ,
				39 ,7  ,47 ,15 ,55 ,23 ,63 ,31 ,
				38 ,6  ,46 ,14 ,54 ,22 ,62 ,30 ,
				37 ,5  ,45 ,13 ,53 ,21 ,61 ,29 ,
				36 ,4  ,44 ,12 ,52 ,20 ,60 ,28 ,
				35 ,3  ,43 ,11 ,51 ,19 ,59 ,27 ,
				34 ,2  ,42 ,10 ,50 ,18 ,58 ,26 ,
				33 ,1  ,41 ,9  ,49 ,17 ,57 ,25 };
    int bit;
    unsigned long int next = 0;
    for(int i = 0; i < 64; i++){
        bit = (encrypted & ( 1 << i )) >> i;    //get bit from initial key
        if(bit == 1){
            next = next | 1 << (invPerm[i]-1);     //set bit i if bit i in intital key is 1
        }
    }
    //R16, take bits 32-63 and put them into 0-32
    for(int i = 0; i < 32; i++){
        bit = (next & ( 1 << (i+32) )) >> (i+32);
        if(bit == 1){
            rSide[16] = rSide[16] | i << i;
        }
    }

    //L16, clear bits 32-63
    lSide[16] = next;
    for(int i = 32; i < 64; i++){
        bit = (lSide[16] & ( 1 << i )) >> i;
        if(bit == 1){
            lSide[16] = lSide[16] & ~ (1 << i);
        }
    }
    decryptLeftRight(lSide, rSide, keyArray);
    unsigned long int afterPerm = lSide[0];
    afterPerm = (afterPerm<<32) | rSide[0];

    int ipTable[64] = {58, 50, 42, 34, 26, 18, 10, 2,
				    60, 52, 44, 36, 28, 20, 12, 4,
                    62, 54, 46, 38, 30, 22, 14, 6,
                    64, 56, 48, 40, 32, 24, 16, 8,
                    57, 49, 41, 33, 25, 17,  9, 1,
                    59, 51, 43, 35, 27, 19, 11, 3,
                    61, 53, 45, 37, 29, 21, 13, 5,
                    63, 55, 47, 39, 31, 23, 15, 7};
    unsigned long int original = 0;
    for(int i = 0; i < 64; i++){
        bit = (afterPerm & ( 1 << i )) >> i;    //get bit from initial key
        if(bit == 1){
            original = original | 1 << (ipTable[i]-1);     //set bit i if bit i in intital key is 1
        }
    }
    return original;
}
void decryptLeftRight(unsigned long int left[17], unsigned long int right[17], unsigned long int keys[16]){
    for(int i = 15; i > 0; i--){
        //left[i] = right[i-1];
        right[i] = left[i+1];
        //right[i] = left[i-1] ^ functionF(right[i-1], keys[i]);
        left[i] = right[i+1] ^ undoF(right[i], keys[i]);
    }
}
unsigned long int undoF(unsigned long int finalResult, unsigned long int key){
    int pTable[32] = {16,  7, 20, 21, 29, 12, 28, 17,1, 15, 23, 26,5, 18, 31, 10,
					2,  8, 24, 14, 32, 27,  3,  9, 19, 13, 30,  6,22, 11,  4, 25};
    unsigned long int eR = 0, eRxorK = 0, beforeP = 0, right = 0;
    int bit = 0;
    int sResult[8] = {0,0,0,0,0,0,0,0};
    int row = 0, column = 0;

    for(int i = 0; i < 32; i++){
        bit = (finalResult & ( 1 << i )) >> i; 
        if(bit == 1){
            beforeP = beforeP | 1 << (pTable[i]-1);  
        }
    }
    for(int i = 0; i < 8; i++){
        sResult[i] = (sResult[i]<<4) | beforeP;
    }


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
    //========================================================
    //s8
    row = 0, column = 0;
    do{
        row++;
        do{
            column++;
        }while(sResult[7] != s8[row][column]);
    }while(sResult[7] != s8[row][column]);

        //getting row
    bit = (row & ( 1UL << 1 )) >> 1;
    if(bit == 1) eRxorK = eRxorK | 5UL << 5;
    bit = (row & ( 1UL << 0 )) >> 0;
    if(bit == 1) eRxorK = eRxorK | 0UL << 0;
        //getting column
    bit = (column & ( 1UL << 3 )) >> 3;
    if(bit == 1) eRxorK = eRxorK | 4UL << 4;
    bit = (column & ( 1UL << 2 )) >> 2;
    if(bit == 1) eRxorK = eRxorK | 3UL << 3;
    bit = (column & ( 1UL << 1 )) >> 1;
    if(bit == 1) eRxorK = eRxorK | 2UL << 2;
    bit = (column & ( 1UL << 0 )) >> 0;
    if(bit == 1) eRxorK = eRxorK | 1UL << 1;

    //s7
    row = 0, column = 0;
    do{
        row++;
        do{
            column++;
        }while(sResult[7] != s7[row][column]);
    }while(sResult[6] != s7[row][column]);

        //getting row
    bit = (row & ( 1UL << 1 )) >> 1;
    if(bit == 1) eRxorK = eRxorK | 11UL << 11;
    bit = (row & ( 1UL << 0 )) >> 0;
    if(bit == 1) eRxorK = eRxorK | 6UL << 6;
        //getting column
    bit = (column & ( 1UL << 3 )) >> 3;
    if(bit == 1) eRxorK = eRxorK | 10UL << 10;
    bit = (column & ( 1UL << 2 )) >> 2;
    if(bit == 1) eRxorK = eRxorK | 9UL << 9;
    bit = (column & ( 1UL << 1 )) >> 1;
    if(bit == 1) eRxorK = eRxorK | 8UL << 8;
    bit = (column & ( 1UL << 0 )) >> 0;
    if(bit == 1) eRxorK = eRxorK | 7UL << 7;

    //s6
    row = 0, column = 0;
    do{
        row++;
        do{
            column++;
        }while(sResult[5] != s6[row][column]);
    }while(sResult[5] != s6[row][column]);

        //getting row
    bit = (row & ( 1UL << 1 )) >> 1;
    if(bit == 1) eRxorK = eRxorK | 17UL << 17;
    bit = (row & ( 1UL << 0 )) >> 0;
    if(bit == 1) eRxorK = eRxorK | 12UL << 12;
        //getting column
    bit = (column & ( 1UL << 3 )) >> 3;
    if(bit == 1) eRxorK = eRxorK | 16UL << 16;
    bit = (column & ( 1UL << 2 )) >> 2;
    if(bit == 1) eRxorK = eRxorK | 15UL << 15;
    bit = (column & ( 1UL << 1 )) >> 1;
    if(bit == 1) eRxorK = eRxorK | 14UL << 14;
    bit = (column & ( 1UL << 0 )) >> 0;
    if(bit == 1) eRxorK = eRxorK | 13UL << 13;
    
    //s5
    row = 0, column = 0;
    do{
        row++;
        do{
            column++;
        }while(sResult[4] != s5[row][column]);
    }while(sResult[4] != s5[row][column]);

        //getting row
    bit = (row & ( 1UL << 1 )) >> 1;
    if(bit == 1) eRxorK = eRxorK | 23UL << 23;
    bit = (row & ( 1UL << 0 )) >> 0;
    if(bit == 1) eRxorK = eRxorK | 18UL << 18;
        //getting column
    bit = (column & ( 1UL << 3 )) >> 3;
    if(bit == 1) eRxorK = eRxorK | 22UL << 22;
    bit = (column & ( 1UL << 2 )) >> 2;
    if(bit == 1) eRxorK = eRxorK | 21UL << 21;
    bit = (column & ( 1UL << 1 )) >> 1;
    if(bit == 1) eRxorK = eRxorK | 20UL << 20;
    bit = (column & ( 1UL << 0 )) >> 0;
    if(bit == 1) eRxorK = eRxorK | 19UL << 19;
    
    //s4
    row = 0, column = 0;
    do{
        row++;
        do{
            column++;
        }while(sResult[3] != s4[row][column]);
    }while(sResult[3] != s4[row][column]);

        //getting row
    bit = (row & ( 1UL << 1 )) >> 1;
    if(bit == 1) eRxorK = eRxorK | 29UL << 29;
    bit = (row & ( 1UL << 0 )) >> 0;
    if(bit == 1) eRxorK = eRxorK | 24UL << 24;
        //getting column
    bit = (column & ( 1UL << 3 )) >> 3;
    if(bit == 1) eRxorK = eRxorK | 28UL << 28;
    bit = (column & ( 1UL << 2 )) >> 2;
    if(bit == 1) eRxorK = eRxorK | 27UL << 27;
    bit = (column & ( 1UL << 1 )) >> 1;
    if(bit == 1) eRxorK = eRxorK | 26UL << 26;
    bit = (column & ( 1UL << 0 )) >> 0;
    if(bit == 1) eRxorK = eRxorK | 25UL << 25;
    
    //s3
    row = 0, column = 0;
    do{
        row++;
        do{
            column++;
        }while(sResult[2] != s3[row][column]);
    }while(sResult[2] != s3[row][column]);

        //getting row
    bit = (row & ( 1UL << 1 )) >> 1;
    if(bit == 1) eRxorK = eRxorK | 35UL << 35;
    bit = (row & ( 1UL << 0 )) >> 0;
    if(bit == 1) eRxorK = eRxorK | 30UL << 30;
        //getting column
    bit = (column & ( 1UL << 3 )) >> 3;
    if(bit == 1) eRxorK = eRxorK | 34UL << 34;
    bit = (column & ( 1UL << 2 )) >> 2;
    if(bit == 1) eRxorK = eRxorK | 33UL << 33;
    bit = (column & ( 1UL << 1 )) >> 1;
    if(bit == 1) eRxorK = eRxorK | 32UL << 32;
    bit = (column & ( 1UL << 0 )) >> 0;
    if(bit == 1) eRxorK = eRxorK | 31UL << 31;
    
    //s2
    row = 0, column = 0;
    do{
        row++;
        do{
            column++;
        }while(sResult[1] != s2[row][column]);
    }while(sResult[1] != s2[row][column]);

        //getting row
    bit = (row & ( 1UL << 1 )) >> 1;
    if(bit == 1) eRxorK = eRxorK | 41UL << 41;
    bit = (row & ( 1UL << 0 )) >> 0;
    if(bit == 1) eRxorK = eRxorK | 36UL << 36;
        //getting column
    bit = (column & ( 1UL << 3 )) >> 3;
    if(bit == 1) eRxorK = eRxorK | 40UL << 40;
    bit = (column & ( 1UL << 2 )) >> 2;
    if(bit == 1) eRxorK = eRxorK | 39UL << 39;
    bit = (column & ( 1UL << 1 )) >> 1;
    if(bit == 1) eRxorK = eRxorK | 38UL << 38;
    bit = (column & ( 1UL << 0 )) >> 0;
    if(bit == 1) eRxorK = eRxorK | 37UL << 37;
    
    //s1
    row = 0, column = 0;
    do{
        row++;
        do{
            column++;
        }while(sResult[0] != s1[row][column]);
    }while(sResult[0] != s1[row][column]);

        //getting row
    bit = (row & ( 1UL << 1 )) >> 1;
    if(bit == 1) eRxorK = eRxorK | 47UL << 47;
    bit = (row & ( 1UL << 0 )) >> 0;
    if(bit == 1) eRxorK = eRxorK | 42UL << 42;
        //getting column
    bit = (column & ( 1UL << 3 )) >> 3;
    if(bit == 1) eRxorK = eRxorK | 46UL << 46;
    bit = (column & ( 1UL << 2 )) >> 2;
    if(bit == 1) eRxorK = eRxorK | 45UL << 45;
    bit = (column & ( 1UL << 1 )) >> 1;
    if(bit == 1) eRxorK = eRxorK | 44UL << 44;
    bit = (column & ( 1UL << 0 )) >> 0;
    if(bit == 1) eRxorK = eRxorK | 43UL << 43;
    //========================================================
    eR = eRxorK ^ key;

    int eTable[] =  {32,1,2,3,4,5,4,5,6,7,8,9,8,9,10,11,12,13,12,13,14,15,16,17,
                    16,17,18,19,20,21,20,21,22,23,24,25,24,25,26,27,28,29,28,29,30,31,32,1};
    for(int i = 0; i < 56; i++){
        bit = (eR & ( 1 << i )) >> i;    
        if(bit == 1){
            right = right | 1 << (eTable[i]-1);     
        }
    }
    return right;
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
    long unsigned int final = rSide[16], result = 0;
    int invPerm[64] = { 	40 ,8  ,48 ,16 ,56 ,24 ,64 ,32 ,
				39 ,7  ,47 ,15 ,55 ,23 ,63 ,31 ,
				38 ,6  ,46 ,14 ,54 ,22 ,62 ,30 ,
				37 ,5  ,45 ,13 ,53 ,21 ,61 ,29 ,
				36 ,4  ,44 ,12 ,52 ,20 ,60 ,28 ,
				35 ,3  ,43 ,11 ,51 ,19 ,59 ,27 ,
				34 ,2  ,42 ,10 ,50 ,18 ,58 ,26 ,
				33 ,1  ,41 ,9  ,49 ,17 ,57 ,25 };
    final = (final<<32) | lSide[16];

    
    for(int i = 0; i < 64; i++){
        bit = (final & ( 1 << (invPerm[i]-1) )) >> (invPerm[i]-1);    //get bit from final before invPerm
        if(bit == 1){
            result = result | 1 << i;     //set bit i if bit i in final is 1
        }
    }
    return result;

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

    
    int sResult[8] = {0,0,0,0,0,0,0,0};
    int row = 0, column = 0;

    //s1
        //getting row
    bit = (eRxorK & ( 1UL << 47 )) >> 47;
    if(bit == 1) row = row | 1 << 1;
    bit = (eRxorK & ( 1UL << 42 )) >> 42;
    if(bit == 1) row = row | 0 << 0;
        //getting column
    bit = (eRxorK & ( 1UL << 46 )) >> 46;
    if(bit == 1) column = column | 3 << 3;
    bit = (eRxorK & ( 1UL << 45 )) >> 45;
    if(bit == 1) column = column | 2 << 2;
    bit = (eRxorK & ( 1UL << 44 )) >> 44;
    if(bit == 1) column = column | 1 << 1;
    bit = (eRxorK & ( 1UL << 43 )) >> 43;
    if(bit == 1) column = column | 0 << 0;

    sResult[0] = s1[row][column];

    //s2
    row = 0, column = 0;
        //getting row
    bit = (eRxorK & ( 1UL << 41 )) >> 41;
    if(bit == 1) row = row | 1 << 1;
    bit = (eRxorK & ( 1UL << 36 )) >> 36;
    if(bit == 1) row = row | 0 << 0;
        //getting column
    bit = (eRxorK & ( 1UL << 40 )) >> 40;
    if(bit == 1) column = column | 3 << 3;
    bit = (eRxorK & ( 1UL << 39 )) >> 39;
    if(bit == 1) column = column | 2 << 2;
    bit = (eRxorK & ( 1UL << 38 )) >> 38;
    if(bit == 1) column = column | 1 << 1;
    bit = (eRxorK & ( 1UL << 37 )) >> 37;
    if(bit == 1) column = column | 0 << 0;

    sResult[1] = s2[row][column];

    //s3
    row = 0, column = 0;
        //getting row
    bit = (eRxorK & ( 1UL << 35 )) >> 35;
    if(bit == 1) row = row | 1 << 1;
    bit = (eRxorK & ( 1UL << 30 )) >> 30;
    if(bit == 1) row = row | 0 << 0;
        //getting column
    bit = (eRxorK & ( 1UL << 34 )) >> 34;
    if(bit == 1) column = column | 3 << 3;
    bit = (eRxorK & ( 1UL << 33 )) >> 33;
    if(bit == 1) column = column | 2 << 2;
    bit = (eRxorK & ( 1UL << 32 )) >> 32;
    if(bit == 1) column = column | 1 << 1;
    bit = (eRxorK & ( 1UL << 31 )) >> 31;
    if(bit == 1) column = column | 0 << 0;

    sResult[2] = s3[row][column];

    //s4
    row = 0, column = 0;
        //getting row
    bit = (eRxorK & ( 1UL << 29 )) >> 29;
    if(bit == 1) row = row | 1 << 1;
    bit = (eRxorK & ( 1UL << 24 )) >> 24;
    if(bit == 1) row = row | 0 << 0;
        //getting column
    bit = (eRxorK & ( 1UL << 28 )) >> 28;
    if(bit == 1) column = column | 3 << 3;
    bit = (eRxorK & ( 1UL << 27 )) >> 27;
    if(bit == 1) column = column | 2 << 2;
    bit = (eRxorK & ( 1UL << 26 )) >> 26;
    if(bit == 1) column = column | 1 << 1;
    bit = (eRxorK & ( 1UL << 25 )) >> 25;
    if(bit == 1) column = column | 0 << 0;

    sResult[3] = s4[row][column];

    //s5
    row = 0, column = 0;
        //getting row
    bit = (eRxorK & ( 1UL << 23 )) >> 23;
    if(bit == 1) row = row | 1 << 1;
    bit = (eRxorK & ( 1UL << 18 )) >> 18;
    if(bit == 1) row = row | 0 << 0;
        //getting column
    bit = (eRxorK & ( 1UL << 22 )) >> 22;
    if(bit == 1) column = column | 3 << 3;
    bit = (eRxorK & ( 1UL << 21 )) >> 21;
    if(bit == 1) column = column | 2 << 2;
    bit = (eRxorK & ( 1UL << 20 )) >> 20;
    if(bit == 1) column = column | 1 << 1;
    bit = (eRxorK & ( 1UL << 19 )) >> 19;
    if(bit == 1) column = column | 0 << 0;

    sResult[4] = s5[row][column];

    //s6
    row = 0, column = 0;
        //getting row
    bit = (eRxorK & ( 1UL << 17 )) >> 17;
    if(bit == 1) row = row | 1 << 1;
    bit = (eRxorK & ( 1UL << 12 )) >> 12;
    if(bit == 1) row = row | 0 << 0;
        //getting column
    bit = (eRxorK & ( 1UL << 16 )) >> 16;
    if(bit == 1) column = column | 3 << 3;
    bit = (eRxorK & ( 1UL << 15 )) >> 15;
    if(bit == 1) column = column | 2 << 2;
    bit = (eRxorK & ( 1UL << 14 )) >> 14;
    if(bit == 1) column = column | 1 << 1;
    bit = (eRxorK & ( 1UL << 13 )) >> 13;
    if(bit == 1) column = column | 0 << 0;

    sResult[5] = s6[row][column];

    //s7
    row = 0, column = 0;
        //getting row
    bit = (eRxorK & ( 1UL << 11 )) >> 11;
    if(bit == 1) row = row | 1 << 1;
    bit = (eRxorK & ( 1UL << 6 )) >> 6;
    if(bit == 1) row = row | 0 << 0;
        //getting column
    bit = (eRxorK & ( 1UL << 10 )) >> 10;
    if(bit == 1) column = column | 3 << 3;
    bit = (eRxorK & ( 1UL << 9 )) >> 9;
    if(bit == 1) column = column | 2 << 2;
    bit = (eRxorK & ( 1UL << 8 )) >> 8;
    if(bit == 1) column = column | 1 << 1;
    bit = (eRxorK & ( 1UL << 7 )) >> 7;
    if(bit == 1) column = column | 0 << 0;

    sResult[6] = s7[row][column];

    //s8
    row = 0, column = 0;
        //getting row
    bit = (eRxorK & ( 1UL << 5 )) >> 5;
    if(bit == 1) row = row | 1 << 1;
    bit = (eRxorK & ( 1UL << 0 )) >> 0;
    if(bit == 1) row = row | 0 << 0;
        //getting column
    bit = (eRxorK & ( 1UL << 4 )) >> 4;
    if(bit == 1) column = column | 3 << 3;
    bit = (eRxorK & ( 1UL << 3 )) >> 3;
    if(bit == 1) column = column | 2 << 2;
    bit = (eRxorK & ( 1UL << 2 )) >> 2;
    if(bit == 1) column = column | 1 << 1;
    bit = (eRxorK & ( 1UL << 1 )) >> 1;
    if(bit == 1) column = column | 0 << 0;

    sResult[7] = s8[row][column];

    //combine sResults together
    for(int i = 0; i < 8; i++){
        beforeP = (beforeP<<4) | sResult[i];
    }

    //put through P table
    int pTable[32] = {16,  7, 20, 21, 29, 12, 28, 17,1, 15, 23, 26,5, 18, 31, 10,
					2,  8, 24, 14, 32, 27,  3,  9, 19, 13, 30,  6,22, 11,  4, 25};
    for(int i = 0; i < 32; i++){
        bit = (beforeP & ( 1 << (pTable[i]-1) )) >> (pTable[i]-1);    //get bit from initial key
        if(bit == 1){
            finalResult = finalResult | 1 << i;     //set bit i if bit i in intital key is 1
        }
    }

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