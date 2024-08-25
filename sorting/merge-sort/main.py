from typing import List

def mergesort(unsorted: List[int]):
    if len(unsorted) <= 1:
        return unsorted
    # split the array
    half_length = len(unsorted)//2
    first_half = unsorted[:half_length]
    second_half = unsorted[half_length:]
    sorted_first_half = mergesort(first_half)
    sorted_second_half = mergesort(second_half)
    final_array = []
    first = 0
    second = 0
    while (first < len(sorted_first_half) and second < len(sorted_second_half)):
        if sorted_first_half[first] <= sorted_second_half[second]:
            final_array.append(sorted_first_half[first])
            first += 1
        else:
            final_array.append(sorted_second_half[second])
            second += 1
    while (first < len(sorted_first_half)):
        final_array.append(sorted_first_half[first])
        first += 1
    while (second < len(sorted_second_half)):
        final_array.append(sorted_second_half[second])
        second += 1
    return final_array

if __name__ == "__main__":
    arr = [3,12,4,21,5,21,4,12,41,24,124,54372,0,23462,34,23426,34,34623,4,236236,12]
    print(mergesort(arr))