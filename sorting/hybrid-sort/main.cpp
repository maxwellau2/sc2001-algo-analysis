#include <iostream>
#include <vector>
#include <chrono>
#include <fstream>
#include <thread>

using std::cout, std::vector;
using std::chrono::high_resolution_clock, std::chrono::duration_cast, std::chrono::nanoseconds;

vector<int> mergesort(vector<int> unsorted);
vector<int> hybridSort(vector<int> unsorted, int threshold);
vector<int> insertionSort(vector<int> unsorted);
void printVector(vector<int>);
void testSorting();
void swap(int*a, int*b);
void timeHybridSort();
void timeMergeSort();
void timeInsertionSort();
void timeInsertionMergeSorts();

int main(int argc, char* argv[]){
    std::thread thread1(timeMergeSort);
    std::thread thread2(timeInsertionSort);
    std::thread thread3(timeHybridSort);

    thread1.join();
    thread2.join();
    thread3.join();
    
    cout << "All sorting operations completed.\n";
    return 0;
}

void timeHybridSort(){
    vector<int> res;
    std::ofstream file;
    file.open("timingsHybrid.csv", std::ios::app); // set to append mode
    file << "sampleSize,hybridSort\n";
    cout << "starting HybridSort timing\n";
    for (int i=0; i<5000; i++){
        // create the sample data
        vector<int> test = {};
        for (int j=i; j>0; j--){
            test.push_back(j);
        }
        // time insertion sort;
        auto startHybridSort = high_resolution_clock::now();
        res = hybridSort(test, 305);
        auto stopHybridSort = high_resolution_clock::now();
        auto durationHybridSort = duration_cast<nanoseconds>(stopHybridSort-startHybridSort);
        file << test.size() << "," << (int)durationHybridSort.count() << "\n";
    }
}

// void timeInsertionMergeSorts(){
//     vector<int> res;
//     std::ofstream file;
//     file.open("timings.csv", std::ios::app); // set to append mode
//     file << "sampleSize,insertionSort,mergeSort\n";
    
//     for (int i=0; i<5000; i++){
//         // create the sample data
//         vector<int> test = {};
//         for (int j=i; j>0; j--){
//             test.push_back(j);
//         }
//         // time insertion sort;
//         auto startInsertionSort = high_resolution_clock::now();
//         res = insertionSort(test);
//         auto stopInsertionSort = high_resolution_clock::now();
//         auto durationInsertionSort = duration_cast<nanoseconds>(stopInsertionSort-startInsertionSort);

//         // time merge sort;
//         auto startMergeSort = high_resolution_clock::now();
//         res = mergesort(test);
//         auto stopMergeSort = high_resolution_clock::now();
//         auto durationMergeSort = duration_cast<nanoseconds>(stopMergeSort-startMergeSort);
//         file << test.size() << "," << (int)durationInsertionSort.count() << "," << (int)durationMergeSort.count() << "\n";
//     }
// }

void timeMergeSort(){
    vector<int> res;
    std::ofstream file;
    file.open("timingsMerge.csv", std::ios::app); // set to append mode
    file << "sampleSize,mergeSort\n";
    cout << "starting Merge Sort timing...\n";
    for (int i=0; i<5000; i++){
        // create the sample data
        vector<int> test = {};
        for (int j=i; j>0; j--){
            test.push_back(j);
        }
        // time merge sort;
        auto startMergeSort = high_resolution_clock::now();
        res = mergesort(test);
        auto stopMergeSort = high_resolution_clock::now();
        auto durationMergeSort = duration_cast<nanoseconds>(stopMergeSort-startMergeSort);
        file << test.size() << "," << (int)durationMergeSort.count() << "\n";
    }
    cout << "Merge Sort Done!\n";
}

void timeInsertionSort(){
    vector<int> res;
    std::ofstream file;
    file.open("timingsInsertion.csv", std::ios::app); // set to append mode
    file << "sampleSize,insertionSort\n";

    cout << "starting Insertion Sort timing...\n";
    for (int i=0; i<5000; i++){
        // create the sample data
        vector<int> test = {};
        for (int j=i; j>0; j--){
            test.push_back(j);
        }
        // time insertion sort;
        auto startInsertionSort = high_resolution_clock::now();
        res = insertionSort(test);
        auto stopInsertionSort = high_resolution_clock::now();
        auto durationInsertionSort = duration_cast<nanoseconds>(stopInsertionSort-startInsertionSort);
        file << test.size() << "," << (int)durationInsertionSort.count() << "\n";
    }
    cout << "Insertion Sort Done!\n";

}

void testSorting() {
    // Test case 1: Already sorted array
    vector<int> sorted_1 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    vector<int> result_hybrid_1 = hybridSort(sorted_1, 2);
    vector<int> result_merge_1 = mergesort(sorted_1);
    vector<int> result_insertion_1 = insertionSort(sorted_1);
    cout << "Test case 1: Already sorted array\n";
    printVector(result_hybrid_1);
    printVector(result_merge_1);
    printVector(result_insertion_1);

    // Test case 2: Reverse sorted array
    vector<int> unsorted_2 = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
    vector<int> result_hybrid_2 = hybridSort(unsorted_2, 2);
    vector<int> result_merge_2 = mergesort(unsorted_2);
    vector<int> result_insertion_2 = insertionSort(unsorted_2);
    cout << "Test case 2: Reverse sorted array\n";
    printVector(result_hybrid_2);
    printVector(result_merge_2);
    printVector(result_insertion_2);

    // Test case 3: Random array
    vector<int> unsorted_3 = {3, 6, 2, 8, 4, 7, 1, 10, 5, 9};
    vector<int> result_hybrid_3 = hybridSort(unsorted_3, 2);
    vector<int> result_merge_3 = mergesort(unsorted_3);
    vector<int> result_insertion_3 = insertionSort(unsorted_3);
    cout << "Test case 3: Random array\n";
    printVector(result_hybrid_3);
    printVector(result_merge_3);
    printVector(result_insertion_3);

    // Test case 4: Empty array
    vector<int> unsorted_4 = {};
    vector<int> result_hybrid_4 = hybridSort(unsorted_4, 2);
    vector<int> result_merge_4 = mergesort(unsorted_4);
    vector<int> result_insertion_4 = insertionSort(unsorted_4);
    cout << "Test case 4: Empty array\n";
    printVector(result_hybrid_4);
    printVector(result_merge_4);
    printVector(result_insertion_4);

    // Test case 5: Single element array
    vector<int> unsorted_5 = {42};
    vector<int> result_hybrid_5 = hybridSort(unsorted_5, 2);
    vector<int> result_merge_5 = mergesort(unsorted_5);
    vector<int> result_insertion_5 = insertionSort(unsorted_5);
    cout << "Test case 5: Single element array\n";
    printVector(result_hybrid_5);
    printVector(result_merge_5);
    printVector(result_insertion_5);

    // Test case 6: Array with duplicates
    vector<int> unsorted_6 = {5, 3, 8, 3, 9, 1, 5, 3, 2, 8};
    vector<int> result_hybrid_6 = hybridSort(unsorted_6, 2);
    vector<int> result_merge_6 = mergesort(unsorted_6);
    vector<int> result_insertion_6 = insertionSort(unsorted_6);
    cout << "Test case 6: Array with duplicates\n";
    printVector(result_hybrid_6);
    printVector(result_merge_6);
    printVector(result_insertion_6);

    // Assert that all results match the expected sorted vector for each case
    vector<int> expected = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    auto assertEqual = [](vector<int> &result, vector<int> &expected, const std::string &testName) {
        if (result == expected) {
            cout << testName << " passed.\n";
        } else {
            cout << testName << " failed.\n";
        }
    };

    assertEqual(result_hybrid_1, expected, "HybridSort Test Case 1");
    assertEqual(result_merge_1, expected, "MergeSort Test Case 1");
    assertEqual(result_insertion_1, expected, "InsertionSort Test Case 1");

    assertEqual(result_hybrid_2, expected, "HybridSort Test Case 2");
    assertEqual(result_merge_2, expected, "MergeSort Test Case 2");
    assertEqual(result_insertion_2, expected, "InsertionSort Test Case 2");

    assertEqual(result_hybrid_3, expected, "HybridSort Test Case 3");
    assertEqual(result_merge_3, expected, "MergeSort Test Case 3");
    assertEqual(result_insertion_3, expected, "InsertionSort Test Case 3");

    vector<int> expected_4 = {};
    assertEqual(result_hybrid_4, expected_4, "HybridSort Test Case 4");
    assertEqual(result_merge_4, expected_4, "MergeSort Test Case 4");
    assertEqual(result_insertion_4, expected_4, "InsertionSort Test Case 4");

    vector<int> expected_5 = {42};
    assertEqual(result_hybrid_5, expected_5, "HybridSort Test Case 5");
    assertEqual(result_merge_5, expected_5, "MergeSort Test Case 5");
    assertEqual(result_insertion_5, expected_5, "InsertionSort Test Case 5");

    vector<int> expected_6 = {1, 2, 3, 3, 3, 5, 5, 8, 8, 9};
    assertEqual(result_hybrid_6, expected_6, "HybridSort Test Case 6");
    assertEqual(result_merge_6, expected_6, "MergeSort Test Case 6");
    assertEqual(result_insertion_6, expected_6, "InsertionSort Test Case 6");
}


void printVector(vector<int> v){
    for(int i: v){
        cout << i << "\t";
    }
    cout << "\n";
    return;
}

vector<int> mergesort(vector<int> unsorted){
    // base case: vector has 1/0 elements, it is already sorted
    if (unsorted.size() <= 1){
        return unsorted;
    }

    // split vector into 2
    int halfLen = unsorted.size()/2;
    // vector slicing can be done using the copy constructor
    // https://stackoverflow.com/questions/50549611/slicing-a-vector-in-c
    auto startPtr = unsorted.begin();
    vector<int> firstHalf = vector<int> (startPtr, startPtr + halfLen);
    vector<int> secondHalf = vector<int> (startPtr + halfLen, unsorted.end());

    vector<int> sortedFirstHalf = mergesort(firstHalf);
    vector<int> sortedSecondHalf = mergesort(secondHalf);

    // create the resulting vector to place elements
    vector<int> result = {};
    int x = 0;
    int y = 0;
    // do the insertions here
    int firstHalfSize = sortedFirstHalf.size();
    int secondHalfSize = sortedSecondHalf.size();
    while (x < firstHalfSize && y < secondHalfSize){
        if (sortedFirstHalf[x] <= sortedSecondHalf[y]){

            result.push_back(sortedFirstHalf[x++]);
        }
        else {
            result.push_back(sortedSecondHalf[y++]);
        }
    }
    // finish up any remaining elements
    while (x < firstHalfSize){
        result.push_back(sortedFirstHalf[x++]);
    }
    while (y < secondHalfSize){
        result.push_back(sortedSecondHalf[y++]);
    }
    return result;
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

vector<int> hybridSort(vector<int> unsorted, int threshold){
    if (unsorted.size() <= 1){
        return unsorted;
    }

    // split vector into 2
    int halfLen = unsorted.size()/2;
    // vector slicing can be done using the copy constructor
    // https://stackoverflow.com/questions/50549611/slicing-a-vector-in-c
    auto startPtr = unsorted.begin();
    vector<int> firstHalf = vector<int> (startPtr, startPtr + halfLen);
    vector<int> secondHalf = vector<int> (startPtr + halfLen, unsorted.end());

    vector<int> sortedFirstHalf;
    vector<int> sortedSecondHalf;

    if (halfLen <= threshold){
        sortedFirstHalf = insertionSort(firstHalf);
        sortedSecondHalf = insertionSort(secondHalf);
    }
    else{
        sortedFirstHalf = hybridSort(firstHalf, threshold);
        sortedSecondHalf = hybridSort(secondHalf, threshold);
    }

    // create the resulting vector to place elements
    vector<int> result = {};
    int x = 0;
    int y = 0;

    // do the insertions here
    int firstHalfSize = sortedFirstHalf.size();
    int secondHalfSize = sortedSecondHalf.size();
    while (x < firstHalfSize && y < secondHalfSize){
        if (sortedFirstHalf[x] <= sortedSecondHalf[y]){

            result.push_back(sortedFirstHalf[x++]);
        }
        else {
            result.push_back(sortedSecondHalf[y++]);
        }
    }
    // finish up any remaining elements
    while (x < firstHalfSize){
        result.push_back(sortedFirstHalf[x++]);
    }
    while (y < secondHalfSize){
        result.push_back(sortedSecondHalf[y++]);
    }
    return result;
}