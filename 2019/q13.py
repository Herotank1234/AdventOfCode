def decode(instr):
  modes = {0 : 0, 1 : 0, 2 : 0}
  op = instr[-2:]
  params = list(instr[:-2])
  params.reverse()
  for i in range(len(params)):
    modes[i] = int(params[i])
  values = [modes[i] for i in range(len(modes))]
  return int(op), values

def advancedRunPlus(currCodes, inputVal, currIndex, relBase):
  index = currIndex
  relativeBase = relBase
  while index < len(currCodes):
    op, modes = decode(str(currCodes[index]))
    # print(op, modes, index)
    if op in [1, 2]:
      val1 = currCodes[index + 1]
      if modes[0] == 0:
        val1 = currCodes[val1]
      elif modes[0] == 2:
        val1 = currCodes[relativeBase + val1]

      val2 = currCodes[index + 2]
      if modes[1] == 0:
        val2 = currCodes[val2]
      elif modes[1] == 2:
        val2 = currCodes[relativeBase + val2]

      val3 = currCodes[index + 3]
      if modes[2] == 2:
        val3 += relativeBase

      if op == 1:
        currCodes[val3] = val1 + val2
      else:
        currCodes[val3] = val1 * val2
      index += 4
    elif op == 3:
      if modes[0] == 0:
        currCodes[currCodes[index + 1]] = inputVal
      else:
        currCodes[currCodes[index + 1] + relativeBase] = inputVal
      index += 2

    elif op == 4:
      if modes[0] == 0:
        return currCodes[currCodes[index + 1]], currCodes, index + 2, relativeBase
      elif modes[0] == 1:
        return currCodes[index + 1], currCodes, index + 2, relativeBase
      else:
        return currCodes[currCodes[index + 1] + relativeBase], currCodes, index + 2, relativeBase
    elif op in [5, 6]:
      val1 = currCodes[index + 1]
      if modes[0] == 0:
        val1 = currCodes[val1]
      elif modes[0] == 2:
        val1 = currCodes[relativeBase + val1]
      val2 = currCodes[index + 2]
      if modes[1] == 0:
        val2 = currCodes[val2]
      elif modes[1] == 2:
        val2 = currCodes[relativeBase + val2]

      if op == 5:
        if val1 != 0:
          index = val2
        else:
          index += 3
      elif op == 6:
        if val1 == 0:
          index = val2
        else:
          index += 3
    elif op in [7, 8]:
      val1 = currCodes[index + 1]
      if modes[0] == 0:
        val1 = currCodes[val1]
      elif modes[0] == 2:
        val1 = currCodes[relativeBase + val1]

      val2 = currCodes[index + 2]
      if modes[1] == 0:
        val2 = currCodes[val2]
      elif modes[1] == 2:
        val2 = currCodes[relativeBase + val2]

      val3 = currCodes[index + 3]
      if modes[2] == 2:
        val3 += relativeBase

      if op == 7:
        if val1 < val2:
          currCodes[val3] = 1
        else:
          currCodes[val3] = 0
      if op == 8:
        if val1 == val2:
          currCodes[val3] = 1
        else:
          currCodes[val3] = 0
      index += 4
    elif op == 9:
      val1 = currCodes[index + 1]
      if modes[0] == 0:
        val1 = currCodes[val1]

      elif modes[0] == 2:
        val1 = currCodes[relativeBase + val1]
      relativeBase += val1
      index += 2
    elif op == 99:
      return -5, currCodes, -5, -5
  return

def part1(codes):
  tiles = {}
  currCodes = codes[:]
  index = 0
  relBase = 0
  while index != -5:
    res1, currCodes, index, relBase = advancedRunPlus(currCodes, 0, index, relBase)
    if index != -5:
      res2, currCodes, index, relBase = advancedRunPlus(currCodes, 0, index, relBase)
      res3, currCodes, index, relBase = advancedRunPlus(currCodes, 0, index, relBase)
    tiles[(res1, res2)] = res3

  total = 0
  for key in tiles:
    if tiles[key] == 2:
      total += 1
  return total

def part2(codes):
  tiles = {}
  currCodes = codes[:]
  currCodes[0] = 2
  joystick = 0
  paddlePos = ballPos = 0
  index = relBase = 0
  res1 = res2 = res3 = 0
  while index != -5:
    res1, currCodes, index, relBase = advancedRunPlus(currCodes, joystick, index, relBase)
    if index != -5:
      res2, currCodes, index, relBase = advancedRunPlus(currCodes, joystick, index, relBase)
      res3, currCodes, index, relBase = advancedRunPlus(currCodes, joystick, index, relBase)
    if res3 == 3:
      paddlePos = res1
    if res3 == 4:
      ballPos = res1

    joystick = 0
    if ballPos > paddlePos:
      joystick = 1
    elif ballPos < paddlePos:
      joystick = -1
  return res3

codes = []
with open("./data/q13.txt", "r") as file:
  for line in file:
    codes = [int(x) for x in line.strip().split(",")]
codes += [0 for _ in range(len(codes))]

print("Answer to part1: {}".format(part1(codes)))
print("Answer to part2: {}".format(part2(codes)))