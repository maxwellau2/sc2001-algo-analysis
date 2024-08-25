from typing import List

def main():
    test = [6,5,4,3,2,1,23,6236,2623,1,21,241,141]
    test = insertion_sort(test)
    print(test)

def insertion_sort(test: List[int]):
    for i in range(1,len(test)):
        for j in range(i,0,-1):
            if test[j] < test[j-1]:
                test[j], test[j-1] = test[j-1], test[j]
            else:
                break
    return test

if __name__ == "__main__":
    main()