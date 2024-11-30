weights = [4, 6, 8]
profits = [7, 6, 9]
n = len(weights)

def knapsack_top_down(capacity, item_idx, memo):
    # If the result is already in memo, return it
    if memo[item_idx][capacity] != -1:
        # print(f"{item_idx} and {capacity} are in memo")
        return memo[item_idx][capacity]

    # print(f"{item_idx} and {capacity} are not in memo")

    # Base case: No remaining capacity or no items left
    if capacity == 0 or item_idx == 0:
        memo[item_idx][capacity] = 0
        return 0

    # Case 1: Item does not fit
    if capacity < weights[item_idx - 1]:  
        memo[item_idx][capacity] = knapsack_top_down(capacity, item_idx - 1, memo)
    else:
        # Case 2: Item fits, choose max of including or excluding it
        take = profits[item_idx - 1] + knapsack_top_down(capacity - weights[item_idx - 1], item_idx, memo)
        exclude = knapsack_top_down(capacity, item_idx - 1, memo)
        memo[item_idx][capacity] = max(take, exclude)

    return memo[item_idx][capacity]

def top_down_driver(C):  # where C is the capacity of the knapsack
    # Initialize memo table with -1
    memo = [[-1 for _ in range(C + 1)] for _ in range(n + 1)]
    return knapsack_top_down(C, n, memo)

def knapsack_bottom_up(C, item_idx):
    table = [[0 for _ in range(C + 1)] for _ in range(n + 1)]
    # Build the table in a bottom-up manner
    for item_idx in range(1, n + 1):
        for capacity_idx in range(C + 1):
            # Case 1: Exclude the current item
            table[item_idx][capacity_idx] = table[item_idx - 1][capacity_idx]

            # Case 2: Include the current item (if it fits)
            if capacity_idx >= weights[item_idx - 1]:
                table[item_idx][capacity_idx] = max(
                    table[item_idx][capacity_idx],
                    profits[item_idx - 1] + table[item_idx][capacity_idx - weights[item_idx - 1]]
                )
    return table[n][C]

def bottom_up_driver(C):
    return knapsack_bottom_up(C, n)

def P(C, mode): # where C is the capacity of the knapsack, mode is 1 for top down and 2 for bottom up
    if mode == 1:
        return top_down_driver(C)
    elif mode == 2: 
        return bottom_up_driver(C)

    # The maximum profit for the given capacity is in dp[n][C]

if __name__ == "__main__":
    print(P(7,1), P(7,2))