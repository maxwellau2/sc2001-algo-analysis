# Question 1

We have a knapsack of **capacity weight C** (a positive integer) and **n types of objects**.
Each object of the **ith type has weight wi and profit pi** (all wi and all pi are positive
integers, i = 0, 1, …, n-1). There are **unlimited supplies** of each type of objects. Find
the largest total profit of any set of the objects that fits in the knapsack.
Let P(C) be the maximum profit that can be made by packing objects into the knapsack
of capacity C

1. Give a recursive definition of the function P(C).

```
global:
    weights = [wi1, wi2, …, win]
    profits = [pi1, pi2, …, pin]

function P_recursive(C, i):
    if i = n:
        return 0
    else:
    // 2 choices: select it, or not select it, if selected, idx will not change
        return max(P_recursive(C-wi, i) + profits[i], P_recursive(C, i+1))

function P(C): // where C is the capacity of the knapsack
    return P_recursive(C, 0)
```

2. Draw the subproblem graph for P(14) where n is 3 with the weights and profits given below

```
global:
    weights = [4, 6, 8]
    profits = [7, 6, 9]
jiawei pls help
```

3. Give a dynamic programming algorithm to compute the maximum profit, given a knapsack of capacity C, n types of objects with weights wi and profits pi using the bottom up approach.

Top Down using Memoization

```python
  # Global variables
weights = [w1, w2, ..., wn]  # weights of items
profits = [p1, p2, ..., pn]  # profits of items
n = len(weights)

def P_recursive(capacity_idx, item_idx, memo):
    # If the result is already in memo, return it
    if memo[item_idx][capacity_idx] != -1:
        return memo[item_idx][capacity_idx]

    # Base case: No remaining capacity or no items left
    if capacity_idx == 0 or item_idx == 0:
        memo[item_idx][capacity_idx] = 0
        return 0

    # Case 1: Item does not fit
    if capacity_idx < weights[item_idx - 1]:
        memo[item_idx][capacity_idx] = P_recursive(capacity_idx, item_idx - 1, memo)
    else:
        # Case 2: Item fits, choose max of including or excluding it
        memo[item_idx][capacity_idx] = max(
            profits[item_idx - 1] + P_recursive(capacity_idx - weights[item_idx - 1], item_idx, memo),
            P_recursive(capacity_idx, item_idx - 1, memo)
        )

    return memo[item_idx][capacity_idx]

def P(C):  # where C is the capacity of the knapsack
    # Initialize memo table with -1
    memo = [[-1 for _ in range(C + 1)] for _ in range(n + 1)]
    return P_recursive(C, n, memo)

```
