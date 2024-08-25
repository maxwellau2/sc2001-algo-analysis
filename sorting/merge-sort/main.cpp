#include <iostream>
#include <vector>

using std::vector, std::cout, std::endl;

vector<int> mergesort(vector<int> unsorted);

int main(){
    vector<int> unsorted = {1,2,3,0,-1,-2,-100};
    // cout << unsorted[1];
    vector<int> result = mergesort(unsorted);
    for (int i=0; i<unsorted.size(); i++){
        cout << result[i] << "\n";
    }
    return 0;
}

vector<int> mergesort(vector<int> unsorted){
    // base case: vector has 1/0 elements, it is already sorted
    if (unsorted.size() <= 1){
        return unsorted;
    }

    // split vector into 2
    int halfLen = unsorted.size()/2;
    vector<int> firstHalf = vector<int> (unsorted.begin(), unsorted.begin() + halfLen);
    vector<int> secondHalf = vector<int> (unsorted.begin() + halfLen, unsorted.end());

    vector<int> sortedFirstHalf = mergesort(firstHalf);
    vector<int> sortedSecondHalf = mergesort(secondHalf);

    // create the resulting vector to place elements
    vector<int> result = {};
    int x = 0;
    int y = 0;
    // do the insertions here
    while (x < sortedFirstHalf.size() && y < sortedSecondHalf.size()){
        if (sortedFirstHalf[x] <= sortedSecondHalf[y]){

            result.push_back(sortedFirstHalf[x++]);
        }
        else {
            result.push_back(sortedSecondHalf[y++]);
        }
    }
    // finish any remaining if have
    while (y < sortedSecondHalf.size()){
        result.push_back(sortedSecondHalf[y++]);
    }
    while (x < sortedFirstHalf.size()){
        result.push_back(sortedFirstHalf[x++]);
    }
    return result;
}