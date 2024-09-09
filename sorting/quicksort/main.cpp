#include <iostream>
#include <vector>

using std::vector, std::cout;

int comparisons = 0;

void printVector(vector<int> v){
    for(int i: v){
        cout << i << "\t";
    }
    cout << "\n";
    return;
}

void swap(int* a, int* b){
    int temp = *a;
    *a=*b;
    *b=temp;
    return;
}

int partition(vector<int>& unsorted, int low, int high) {
    int pivotIndex = low + (high - low) / 2;  // Choose middle element as pivot
    int pivotValue = unsorted[pivotIndex];
    swap(&unsorted[pivotIndex], &unsorted[high]);  // Move pivot to end
    int i = low;  // Place for swapping

    for (int j = low; j < high; j++) {
        if (unsorted[j] < pivotValue) {
            comparisons++;
            swap(&unsorted[i], &unsorted[j]);
            i++;
        }
    }
    swap(&unsorted[i], &unsorted[high]);  // Move pivot to its final place
    return i;  // Return the position of the pivot
}

void quicksort(vector<int>& unsorted, int low, int high) {
    if (low < high) {
        comparisons++;
        int pivotIndex = partition(unsorted, low, high);
        quicksort(unsorted, low, pivotIndex - 1);
        quicksort(unsorted, pivotIndex + 1, high);
    }
}

int main(){
    vector<int> unsorted = {};
    for (int i=99; i>0; i--){
        unsorted.push_back(i);
    }
    // printVector(unsorted);
    quicksort(unsorted, 0, unsorted.size() - 1);
    printVector(unsorted);
    cout << "comparisons: " << comparisons << "\n";

    return 0;
}

