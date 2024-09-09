#include <iostream>
#include <fstream>
#include <vector>

using std::vector, std::cout;

int main(){
    std::ofstream myfile;
    myfile.open("test.csv", std::ios::app);
    myfile << "hello!\n";
    return 0;
}