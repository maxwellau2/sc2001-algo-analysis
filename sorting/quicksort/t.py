def are_neighbours(stone_a, stone_b):
    if stone_a[0] == stone_b[0] or stone_a[1] == stone_b[1]:
        return True
    return False

def dfs(start, graph) -> set:
    visited = set()
    stack = []
    stack.append(start)
    while stack:
        curr = stack.pop()
        visited.add(curr)
        # print(curr)
        for child in graph[curr]:
            if child not in visited:
                stack.append(child)
    return visited


stones = [[0,0],[0,1],[1,0],[1,2],[2,1],[2,2]]
adj_list = {}
for s in stones:
    s_tuple = tuple(s)
    if s_tuple not in adj_list:
        adj_list[s_tuple] = []
        for stone in stones:
            stone_tuple = tuple(stone)
            if stone_tuple != s_tuple and are_neighbours(s_tuple,stone_tuple):
                adj_list[s_tuple].append(stone_tuple)


print(adj_list)
connected = set()
for s in stones:
    visited = dfs(tuple(s), adj_list)
    # connected.add(visited)
    print(visited)
print(len(visited), visited)