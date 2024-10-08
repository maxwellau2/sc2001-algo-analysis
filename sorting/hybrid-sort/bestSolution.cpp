#include <iostream>
#include <vector>
#include <chrono>
#include <fstream>
#include <thread>
#include <mutex>

using std::cout, std::vector;
using std::chrono::high_resolution_clock, std::chrono::duration_cast, std::chrono::nanoseconds;

vector<int> mergesort(vector<int> unsorted);
vector<int> hybridSort(vector<int> unsorted, int threshold);
vector<int> insertionSort(vector<int> unsorted);
vector<int> insertionSortForHybrid(vector<int> unsorted);
void printVector(vector<int>);
void testSorting();
void swap(int*a, int*b);
void timeHybridSort();
void timeMergeSort();
void timeInsertionSort();
void timeInsertionMergeSorts();
void timeHybridSortFixedArarySize();

uint64_t hybridKeyComp = 0;
uint64_t mergeKeyComp = 0;
uint64_t insertKeyComp = 0;

int minSize = 1000;
int maxSize = 10000000;
int step = 5000;

int thresholdTiming = 110;
int thresholdKeyComp = 9;
int trivialThreshold = 200;

int minThreshold = 2;
int maxThreshold = 1000;
int stepThreshold = 1;
int fixedArraySize = 50000;

std::mutex file_mutex;

int main(int argc, char* argv[]){
    std::thread thread1(timeMergeSort);
    // std::thread thread2(timeInsertionSort);
    std::thread thread3(timeHybridSort);

    thread1.join();
    // thread2.join();
    thread3.join();
    // timeHybridSortFixedArarySize();
    
    cout << "All sorting operations completed.\n";
    return 0;
}

void timeHybridSort() {
    vector<int> res;
    std::ofstream file;

    {
        // Use mutex to ensure only one thread writes to the file at a time
        std::lock_guard<std::mutex> lock(file_mutex);
        file.open("timingsHybrid.csv", std::ios::app);
        if (!file.is_open()) {
            cout << "Error opening timingsHybrid.csv for writing.\n";
            return;
        }
        file << "sampleSize,timing,keycomp\n";
    }

    cout << "starting HybridSort timing\n";
    for (int i = 0; i < 20; i ++) {
        cout << "HybridSort timing for " << i << "\n";
        vector<int> test;
        for (int j = 0; j < maxSize; j++) { 
            int randomNum = rand() % maxSize;
            test.push_back(randomNum);
        }

        auto startHybridSort = high_resolution_clock::now();
        res = hybridSort(test, thresholdKeyComp);
        auto stopHybridSort = high_resolution_clock::now();
        auto durationHybridSort = duration_cast<nanoseconds>(stopHybridSort - startHybridSort);

        {
            // Write to file within mutex-protected block
            std::lock_guard<std::mutex> lock(file_mutex);
            file.open("timingsHybrid.csv", std::ios::app);
            if (!file.is_open()) {
                cout << "Error opening timingsHybrid.csv for writing.\n";
                return;
            }
            file << test.size() << "," << durationHybridSort.count() << "," << hybridKeyComp << "\n";
            file.close();
        }

        hybridKeyComp = 0;
    }
    cout << "HybridSort Done!\n";
}

void timeHybridSortFixedArarySize() {
    vector<int> res;
    std::ofstream file;

    {
        // Use mutex to ensure only one thread writes to the file at a time
        std::lock_guard<std::mutex> lock(file_mutex);
        file.open("timingsHybridFixedArraySize50000.csv", std::ios::app);
        if (!file.is_open()) {
            cout << "Error opening timingsHybridFixedArraySize.csv for writing.\n";
            return;
        }
        file << "sampleSize,value,timing,keycomp\n";
    }

    cout << "starting timingsHybridFixedArraySize timing\n";
    for (int i = minThreshold - stepThreshold; i < maxThreshold; i += stepThreshold) {
        cout << "timingsHybridFixedArraySize timing for " << i << "\n";
        vector<int> test;

        // build array of fixed array size
        for (int j = 0; j < fixedArraySize; j++) { 
            int randomNum = rand() % i;
            test.push_back(randomNum);
        }

        // time the sorting
        auto startHybridSort = high_resolution_clock::now();
        res = hybridSort(test, i);
        auto stopHybridSort = high_resolution_clock::now();
        auto durationHybridSort = duration_cast<nanoseconds>(stopHybridSort - startHybridSort);

        {
            // Write to file within mutex-protected block
            std::lock_guard<std::mutex> lock(file_mutex);
            file.open("timingsHybridFixedArraySize50000.csv", std::ios::app);
            if (!file.is_open()) {
                cout << "Error opening timingsHybridFixedArraySize.csv for writing.\n";
                return;
            }
            file << test.size() << ","  << i << "," << durationHybridSort.count() << "," << hybridKeyComp << "\n";
            file.close();
        }

        hybridKeyComp = 0;
    }
    cout << "timingsHybridFixedArraySize Done!\n";
}

void timeMergeSort() {
    vector<int> res;
    std::ofstream file;

    {
        std::lock_guard<std::mutex> lock(file_mutex);
        file.open("timingsMerge.csv", std::ios::app);
        if (!file.is_open()) {
            cout << "Error opening timingsMerge.csv for writing.\n";
            return;
        }
        file << "sampleSize,timing,keycomp\n";
    }

    cout << "starting MergeSort timing\n";
    for (int i = 0; i < 20; i ++){
        cout << "MergeSort timing for " << i << "\n";
        vector<int> test;
        for (int j = 0; j < maxSize; j++) {
            int randomNum = rand() % maxSize;
            test.push_back(randomNum);
        }

        auto startMergeSort = high_resolution_clock::now();
        res = mergesort(test);
        auto stopMergeSort = high_resolution_clock::now();
        auto durationMergeSort = duration_cast<nanoseconds>(stopMergeSort - startMergeSort);

        {
            std::lock_guard<std::mutex> lock(file_mutex);
            file.open("timingsMerge.csv", std::ios::app);
            if (!file.is_open()) {
                cout << "Error opening timingsMerge.csv for writing.\n";
                return;
            }
            file << test.size() << "," << durationMergeSort.count() << "," << mergeKeyComp << "\n";
            file.close();
        }

        mergeKeyComp = 0;
    }
    cout << "MergeSort Done!\n";
}

void timeInsertionSort() {
    vector<int> res;
    std::ofstream file;

    {
        std::lock_guard<std::mutex> lock(file_mutex);
        file.open("timingsInsertion.csv", std::ios::app);
        if (!file.is_open()) {
            cout << "Error opening timingsInsertion.csv for writing.\n";
            return;
        }
        file << "sampleSize,timing,keycomp\n";
    }

    cout << "starting InsertionSort timing\n";
    for (int i = minSize; i < maxSize; i += step) {
        cout << "InsertionSort timing for " << i << "\n";
        vector<int> test;
        for (int j = i; j > 0; j--) {
            int randomNum = rand() % i;
            test.push_back(randomNum);
        }

        auto startInsertionSort = high_resolution_clock::now();
        res = insertionSort(test);
        auto stopInsertionSort = high_resolution_clock::now();
        auto durationInsertionSort = duration_cast<nanoseconds>(stopInsertionSort - startInsertionSort);

        {
            std::lock_guard<std::mutex> lock(file_mutex);
            file.open("timingsInsertion.csv", std::ios::app);
            if (!file.is_open()) {
                cout << "Error opening timingsInsertion.csv for writing.\n";
                return;
            }
            file << test.size() << "," << durationInsertionSort.count() << "," << insertKeyComp << "\n";
            file.close();
        }

        insertKeyComp = 0;
    }
    cout << "InsertionSort Done!\n";
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
    mergeKeyComp++;
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
        mergeKeyComp++;
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
            insertKeyComp++;
            if (unsorted[j] < unsorted[j-1]){
                swap(&unsorted[j], &unsorted[j-1]);
            }
            else break;
        }
    }
    return unsorted;
}

vector<int> insertionSortForHybrid(vector<int> unsorted){
    int temp;
    for (int i=1; i<unsorted.size(); i++){
        for (int j=i; j>0; j--){
            hybridKeyComp++;
            if (unsorted[j] < unsorted[j-1]){
                swap(&unsorted[j], &unsorted[j-1]);
            }
            else break;
        }
    }
    return unsorted;
}

vector<int> hybridSort(vector<int> unsorted, int threshold){
    hybridKeyComp++;
    if (unsorted.size() <= 1){
        return unsorted;
    }
    hybridKeyComp++;
    if (unsorted.size() <= threshold){
        return insertionSortForHybrid(unsorted);
    }

    // split vector into 2
    int halfLen = unsorted.size()/2;
    // vector slicing can be done using the copy constructor
    // https://stackoverflow.com/questions/50549611/slicing-a-vector-in-c
    auto startPtr = unsorted.begin();
    vector<int> firstHalf = vector<int> (startPtr, startPtr + halfLen);
    vector<int> secondHalf = vector<int> (startPtr + halfLen, unsorted.end());

    vector<int> sortedFirstHalf = hybridSort(firstHalf, threshold);
    vector<int> sortedSecondHalf = hybridSort(secondHalf, threshold);

    // create the resulting vector to place elements
    vector<int> result = {};
    int x = 0;
    int y = 0;

    // do the insertions here
    int firstHalfSize = sortedFirstHalf.size();
    int secondHalfSize = sortedSecondHalf.size();
    while (x < firstHalfSize && y < secondHalfSize){
        hybridKeyComp++;
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