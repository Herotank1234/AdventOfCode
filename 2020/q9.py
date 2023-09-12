def part1(nums):
    def _check(currNums: list, val: int) -> bool:
        length = len(currNums)
        for i in range(length - 1):
            for j in range(i, length):
                if currNums[i] + currNums[j] == val:
                    return True
        return False

    currNums = nums[:25]
    currNumsIndex = 0
    index = 25

    while index < len(nums):
        valid = _check(currNums, nums[index])
        if not valid:
            return nums[index]
        else:
            currNums[currNumsIndex] = nums[index]
            index += 1
            currNumsIndex += 1
            if currNumsIndex == 25:
                currNumsIndex %= 25

    return -1

def part2(nums):
    invalidNum = 26134589
    i = 0
    j = 1
    currSum = nums[i] + nums[j]

    while currSum != invalidNum:
        if currSum < invalidNum:
            j += 1
            currSum += nums[j]
        else:
            currSum -= nums[i]
            i += 1

    numsRange = nums[i : j + 1]
    return min(numsRange) + max(numsRange)

nums = []
with open("./data/q9.txt", "r") as file:
    for line in file:
        nums.append(int(line.strip()))

print("Answer to part1: {}".format(part1(nums)))
print("Answer to part2: {}".format(part2(nums)))