import copy

def part1(cups):
  cupsList = copy.deepcopy(cups)
  key = cupsList[0]
  cupsList = dict(list(zip(cupsList, cupsList[1:] + [cupsList[0]])))

  for _ in range(0, 100):
    removeKey = key
    removed = []
    for i in range(0, 3):
      removed.append(cupsList[removeKey])
      removeKey = cupsList[removeKey]
    
    cupsList[key] = cupsList[removed[2]]

    nextCupNumber = (((key - 1) - 1) % 9) + 1
    
    while nextCupNumber in removed:
      nextCupNumber -= 1
      nextCupNumber = ((nextCupNumber - 1) % 9) + 1
    

    nextCupNumberNext = cupsList[nextCupNumber]
    cupsList[nextCupNumber] = removed[0]
    cupsList[removed[2]] = nextCupNumberNext

    key = cupsList[key]

  res = ""
  index = 1
  while cupsList[index] != 1:
    res += str(cupsList[index])
    index = cupsList[index]
  return res

def part2(cups):
  cupsList = copy.deepcopy(cups)
  key = cupsList[0]
  for x in range(10, 1_000_001):
    cupsList.append(x)
  cupsList = dict(list(zip(cupsList, cupsList[1:] + [cupsList[0]])))

  for _ in range(0, 10_000_000):
    removeKey = key
    removed = []
    for i in range(0, 3):
      removed.append(cupsList[removeKey])
      removeKey = cupsList[removeKey]
    
    cupsList[key] = cupsList[removed[2]]

    nextCupNumber = (((key - 1) - 1) % 1_000_000) + 1
    
    while nextCupNumber in removed:
      nextCupNumber -= 1
      nextCupNumber = ((nextCupNumber - 1) % 1_000_000) + 1
    

    nextCupNumberNext = cupsList[nextCupNumber]
    cupsList[nextCupNumber] = removed[0]
    cupsList[removed[2]] = nextCupNumberNext

    key = cupsList[key]
  return cupsList[1] * cupsList[cupsList[1]]

cups = []
with open("./data/q23.txt", "r") as file:
  for line in file:
    cups = [int(x) for x in line.strip()]

print("Answer to part1: {}".format(part1(cups)))
print("Answer to part2: {}".format(part2(cups)))