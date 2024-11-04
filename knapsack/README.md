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
