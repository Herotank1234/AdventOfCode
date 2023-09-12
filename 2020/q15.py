def part1(nums):
  spoken = {}
  length = len(nums)
  lastSpoken = nums[length - 1]

  for i in range(length):
    spoken[nums[i]] = i + 1
  
  for i in range(length, 2020):
    if lastSpoken not in spoken:
      spoken[lastSpoken] = i
      lastSpoken = 0
    else:
      diff = i - spoken[lastSpoken]
      spoken[lastSpoken] = i
      lastSpoken = diff
    
  return lastSpoken

def part2(nums):
  spoken = {}
  length = len(nums)
  lastSpoken = nums[length - 1]

  for i in range(length):
    spoken[nums[i]] = i + 1
  
  for i in range(length, 30000000):
    if lastSpoken not in spoken:
      spoken[lastSpoken] = i
      lastSpoken = 0
    else:
      diff = i - spoken[lastSpoken]
      spoken[lastSpoken] = i
      lastSpoken = diff
    
  return lastSpoken

nums = []
with open("./data/q15.txt", "r") as file:
  for line in file:
    nums = [int(x) for x in line.strip().split(",")]

print("Answer to part1: {}".format(part1(nums)))
print("Answer to part2: {}".format(part2(nums)))