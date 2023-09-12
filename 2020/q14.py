from locale import currency
import re

def part1(maskMems):
  def _getMask(mask):
    maskPos = {}
    for i in range(len(mask)):
      if mask[i] == "1" or mask[i] == "0":
        maskPos[i] = mask[i]
    return maskPos

  index = 0
  maskPos = {}
  memory = {}

  while index < len(maskMems):
    maskPos = _getMask(maskMems[index][1])
    index += 1
    while index < len(maskMems) and maskMems[index][0] == "mem":
      currNum = list(format(maskMems[index][2], "036b"))
      for key in maskPos:
        currNum[key] = maskPos[key]
      memory[maskMems[index][1]] = int("".join(currNum), 2)
      index += 1
  
  total = 0
  for key in memory:
    total += memory[key]
  return total

def part2():
  def _getMask(mask):
    maskPos = {}
    xPos = []
    for i in range(len(mask)):
      if mask[i] == "1" or mask[i] == "X":
        maskPos[i] = mask[i]
        if mask[i] == "X":
          xPos.append(i)
    return maskPos, xPos
  
  def _getAllAddrs(addr, xPos):
    currAddrs = [addr]
    nextAddrs = []
    for x in xPos:
      for i in ["0", "1"]:
        for addr in currAddrs:
          tmp = list(addr)
          tmp[x] = i
          nextAddrs.append("".join(tmp))
      currAddrs = nextAddrs[:]
      nextAddrs = []
    return currAddrs
    

  index = 0
  maskPos = {}
  memory = {}

  while index < len(maskMems):
    maskPos, xPos = _getMask(maskMems[index][1])
    index += 1
    while index < len(maskMems) and maskMems[index][0] == "mem":
      currAddr = list(format(maskMems[index][1], "036b"))
      for key in maskPos:
        currAddr[key] = maskPos[key]
      allAddr = _getAllAddrs("".join(currAddr), xPos)
      for addr in allAddr:
        memory[int(addr, 2)] = maskMems[index][2]
      index += 1
  
  total = 0
  for key in memory:
    total += memory[key]
  return total

maskMems = []
with open("./data/q14.txt", "r") as file:
  for line in file:
    if line[:4] == "mask":
      maskMems.append(line.strip().split(" = "))
    else:
      match = [int(x) for x in re.findall(r"[\d]+", line.strip())]
      maskMems.append(["mem"] + match)

print("Answer to part1: {}".format(part1(maskMems)))
print("Answer to part2: {}".format(part2()))