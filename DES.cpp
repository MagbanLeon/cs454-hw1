#include<iostream>
using namespace std;

 int main(int argc, char* argv[]){
    if(argc != 3){
        cout << "There aren't enough parameters!" << endl << "Enter as './des sample_input.txt output.txt'" << endl;
        return -1;
    }
    
    return 0;
 }