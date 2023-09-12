TARGET = 2020

def part1(nums):
    currNums = nums[:]
    currNums.sort()

    i = 0
    j = 0
    length = len(currNums)
    found = False

    while i < length and not found:
        while j < length and (currNums[i] + currNums[j] < TARGET):
            j += 1
        if (currNums[i] + currNums[j]) == TARGET:
            found = True
        else:
            i += 1
            j = i
    return currNums[i] * currNums[j]

def part2(nums):
    currNums = nums[:]
    currNums.sort()

    i = 0
    j = 1
    k = 2
    length = len(currNums)
    found = False

    while i < length - 2 and not found:
        while j < length - 1 and not found:
            while k < length and (currNums[i] + currNums[j] + currNums[k] < TARGET):
                k += 1
            if (currNums[i] + currNums[j] + currNums[k]) == TARGET:
                found = True
            else:
                j += 1
                k = j + 1
        if not found:
            i += 1
            j = i + 1
            k = j + 1
    
    return currNums[i] * currNums[j] * currNums[k]
nums = []

with open("./data/q1.txt", "r") as file:
    for line in file:
        nums.append(int(line.strip()))

print("Answer to part1: {}".format(part1(nums)))
print("Answer to part2: {}".format(part2(nums)))