#include <iostream>
#include <fstream>

int main(){
    std::ofstream myfile;
    myfile.open("test.csv", std::ios::app);
    myfile << "hello!\n";
    return 0;
}