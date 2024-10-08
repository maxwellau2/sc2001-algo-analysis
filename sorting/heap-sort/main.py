key_comparisons = 0

def max_heapify(arr, heap_size, i):
    l = 2*i + 1
    r = 2*i + 2

    largest = i
    
    if l<heap_size and arr[l] > arr[i]:
        largest = l

    if r < heap_size and arr[r] > arr[largest]:
        largest = r

    if largest != i:
        # swap
        print(f"swapping {arr[i]}, {arr[largest]}")
        arr[i], arr[largest] = arr[largest], arr[i]
        print(f"current array: {arr}")
        max_heapify(arr, heap_size, largest)

def build_max_heap(arr, verbose=False):
    heap_size = len(arr)
    for i in range(heap_size//2, -1, -1): # starting from leaves, going backwards
        max_heapify(arr, heap_size, i, verbose)

def min_heapify(arr, heap_size, i, verbose=False):
    global key_comparisons
    l = 2*i+1
    r = 2*i+2

    smallest = i
    
    if l<heap_size:
        if arr[l] < arr[i]:
            smallest = l
        key_comparisons += 1

    if r < heap_size:
        if arr[r] < arr[smallest]:
            smallest = r
        key_comparisons += 1

    key_comparisons += 1
    if smallest != i:
        # swap
        if verbose:
            print(f"swapping {arr[i]}, {arr[smallest]}")
        arr[i], arr[smallest] = arr[smallest], arr[i]
        if verbose:
            print(f"current array: {arr}")
        min_heapify(arr, heap_size, smallest)

def build_min_heap(arr, verbose=False):
    heap_size = len(arr)
    for i in range(heap_size//2, -1, -1): # starting from leaves, going backwards
        min_heapify(arr, heap_size, i, verbose)

def main():
    # a = [1,2,3,4,5,6,7,8,9,0]
    global key_comparisons
    for i in range(1,11):
        a = list(range(i,0,-1))
        # print(a)
        build_min_heap(a)
        # print(a)
        print(f"num key comparisons {a}: {key_comparisons}")
        key_comparisons = 0

main()