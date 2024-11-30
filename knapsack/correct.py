def uks_rec(W, wt, val, idx):
  
    # Base Case
    if idx == 0:
        return (W // wt[0]) * val[0]

    # There are two cases: either take the 
    # element or not.
    not_take = uks_rec(W, wt, val, idx - 1)

    # if take then weight = W-wt[idx] and 
    # index will remain same.  
    take = float('-inf')
    if wt[idx] <= W:
        take = val[idx] + uks_rec(W - wt[idx], wt, val, idx)

    return max(take, not_take)

# Wrapper method for unbounded knapsack
def uks(W, wt, val):
    n = len(val)
    return uks_rec(W, wt, val, n - 1)


W = 7
val = [7, 6, 9]
wt = [4, 6, 8]

print(uks(W, wt, val))