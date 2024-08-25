#include <iostream>
#include <vector>

using std::cout, std::vector;
void insertionSortInPlace(std::vector<int> *unsorted);
vector<int> insertionSort(vector<int> unsorted);

int main(){
    vector<int> test = {6,5,4,3,2,1,23,6236,2623,1,21,241,141};
    cout << "starting with..\n";
    for(int i=0; i< test.size(); i++){
        std::cout << test[i] << "\n";
    }
    vector<int> res = insertionSort(test);
    cout << "ending with.. \n";
    for(int i=0; i<test.size(); i++){
        cout<< res[i] << "\n";
    }
    return 0;
}


// pseudo code for insertion sort
// for i in [1,n]
//      for j in [i,0]
//          if adjacent elements not in order
//              swap

void insertionSortInPlace(vector<int> *unsorted){
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

void swap(int*a, int*b){
    int temp = *a;
    *a = *b;
    *b = temp;
    return;
}

vector<int> insertionSort(vector<int> unsorted){
    int temp;
    for (int i=1; i<unsorted.size(); i++){
        for (int j=i; j>0; j--){
            if (unsorted[j] < unsorted[j-1]){
                swap(&unsorted[j], &unsorted[j-1]);
            }
            else break;
        }
    }
    return unsorted;
}
