#include <iostream>
#include <vector>
void insertionSort(std::vector<int> *unsorted);

int main(){
    std::vector<int> test = {6,5,4,3,2,1,23,6236,2623,1,21,241,141};
    std::cout << "starting with..\n";
    for(int i=0; i< test.size(); i++){
        std::cout << test[i] << std::endl;
    }
    insertionSort(&test);
    std::cout << "ending with.. \n";
    for(int i=0; i<test.size(); i++){
        std::cout<< test[i] << std::endl;
    }
    return 0;
}

void insertionSort(std::vector<int> *unsorted){
    int temp;
    for(int i=1; i<unsorted->size(); i++){
        for(int j=i; j>0; j--){
            if ((*unsorted)[j] < (*unsorted)[j-1]){
                // swap
                temp = (*unsorted)[j];
                (*unsorted)[j] = (*unsorted)[j-1];
                (*unsorted)[j-1] = temp;
            } else {
                break;
            }
        }
    }
    return;
}