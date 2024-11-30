#include <chrono>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <mutex>
#include <vector>

std::vector<int> weights;
std::vector<int> profits;
int n = weights.size();

int knapsackTopDown(int capacity, int item_idx,
                    std::vector<std::vector<int>>& memo) {
    // If the result is already in memo, return it
    if (memo[item_idx][capacity] != -1) {
        return memo[item_idx][capacity];
    }

    // Base case: No remaining capacity or no items left
    if (capacity == 0 || item_idx == 0) {
        memo[item_idx][capacity] = 0;
        return 0;
    }

    // Case 1: Item does not fit
    if (capacity < weights[item_idx - 1]) {
        memo[item_idx][capacity] =
            knapsackTopDown(capacity, item_idx - 1, memo);
    } else {
        // Case 2: Item fits, choose max of including or excluding it
        int take =
            profits[item_idx - 1] +
            knapsackTopDown(capacity - weights[item_idx - 1], item_idx, memo);
        int exclude = knapsackTopDown(capacity, item_idx - 1, memo);
        memo[item_idx][capacity] = std::max(take, exclude);
    }
    return memo[item_idx][capacity];
}

int knapsackBottomUp(int C, int item_idx) {
    std::vector<std::vector<int>> table(n + 1, std::vector<int>(C + 1, 0));
    // Build the table in a bottom-up manner
    for (int item_idx = 1; item_idx <= n; ++item_idx) {
        for (int capacity_idx = 0; capacity_idx <= C; ++capacity_idx) {
            // Case 1: Exclude the current item
            table[item_idx][capacity_idx] = table[item_idx - 1][capacity_idx];

            // Case 2: Include the current item (if it fits)
            if (capacity_idx >= weights[item_idx - 1]) {
                table[item_idx][capacity_idx] = std::max(
                    table[item_idx][capacity_idx],
                    profits[item_idx - 1] +
                        table[item_idx][capacity_idx - weights[item_idx - 1]]);
            }
        }
    }

    // The maximum profit for the given capacity is in table[n][C]
    return table[n][C];
}

int topDownDriver(int C) {  // where C is the capacity of the knapsack
    // Initialize memo table with -1
    std::vector<std::vector<int>> memo(n + 1, std::vector<int>(C + 1, -1));
    return knapsackTopDown(C, n, memo);
}

int bottomUpDriver(int C) { return knapsackBottomUp(C, n); }

int P(int C, int mode) {  // where C is the capacity of the knapsack, mode is 1
                          // for top down and 2 for bottom up
    if (mode == 1) {
        return topDownDriver(C);
    } else if (mode == 2) {
        return bottomUpDriver(C);
    }

    // The maximum profit for the given capacity is in dp[n][C]
    return 0;
}

void printList(std::vector<int> list) {
    for (int i = 0; i < list.size(); ++i) {
        std::cout << list[i] << " ";
    }
    std::cout << std::endl;
}

void compareDp() {
    srand(69420);
    // initialise the csv write
    std::ofstream file;

    file.open("knapsack_results.csv", std::ios::app);
    file << "Array Size,Top Down Time (us),Bottom Up Time (us)\n";
    file.close();
    // do for array sizes 10 to 10,000,
    for (int i = 10; i <= 10000; i += 10) {
        std::cout << "Array Size: " << i << "\n";
        weights.clear();
        profits.clear();
        // generate random weights and profits
        for (int j = 0; j < i; ++j) {
            weights.push_back(rand() % 10 + 1);
            profits.push_back(rand() % 10 + 1);
        }
        n = weights.size();
        int capacity = rand() % 50 + 1;
        // time the algorithms
        auto startTopDown = std::chrono::high_resolution_clock::now();
        int topDownResult = P(capacity, 1);
        auto endTopDown = std::chrono::high_resolution_clock::now();
        auto durationTopDown =
            std::chrono::duration_cast<std::chrono::microseconds>(endTopDown -
                                                                  startTopDown)
                .count();
        std::cout << "Top Down Time: " << durationTopDown << std::endl;

        auto startBottomUp = std::chrono::high_resolution_clock::now();
        int bottomUpResult = P(capacity, 2);
        auto endBottomUp = std::chrono::high_resolution_clock::now();
        auto durationBottomUp =
            std::chrono::duration_cast<std::chrono::microseconds>(endBottomUp -
                                                                  startBottomUp)
                .count();
        std::cout << "Bottom Up Time: " << durationBottomUp << std::endl;

        // write to file
        file.open("knapsack_results.csv", std::ios::app);
        file << i << "," << durationTopDown << "," << durationBottomUp << "\n";
        file.close();
    }
}

void test1() {
    std::cout << "Test1\n";
    int capacity = 14;
    weights = {4, 6, 8};
    profits = {7, 6, 9};
    n = weights.size();
    int resultTopDown = P(capacity, 1);
    int resultBottomUp = P(capacity, 2);
    std::cout << "Top Down Result: " << resultTopDown << std::endl;
    std::cout << "Bottom Up Result: " << resultBottomUp << std::endl;
    std::cout << "\n";
}

void test2() {
    std::cout << "Test2\n";
    int capacity = 14;
    weights = {5, 6, 8};
    profits = {7, 6, 9};
    n = weights.size();
    int resultTopDown = P(capacity, 1);
    int resultBottomUp = P(capacity, 2);
    std::cout << "Top Down Result: " << resultTopDown << std::endl;
    std::cout << "Bottom Up Result: " << resultBottomUp << std::endl;
    std::cout << "\n";
}

int main() {
    std::cout << "please uncomment the compareDp() function to run the dp \n";
    // compareDp();

    std::cout << "please uncomment the test1() and test2() function to show "
                 "the output \n";
    // test1();
    // test2();

    return 0;
}