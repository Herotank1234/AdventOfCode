from Intcode import Intcode

def part1(codes):
  intcode = Intcode(codes)
  generator = intcode.run()

  grid = []
  row = []
  for value in generator:
    if value != 10:
      row.append(chr(value))
    else:
      grid.append(row)
      row = []

  grid = grid[:-1]
  alignParamSum = 0
  for i in range(1, len(grid) - 1):
    for j in range(1, len(grid[0]) - 1):
      if grid[i][j] == '#' and grid[i - 1][j] == '#' and \
          grid[i + 1][j] == '#' and grid[i][j - 1] == '#' and grid[i][j + 1] == '#':
        alignParamSum += (i * j)

  return alignParamSum

def getPos(grid):
  for i in range(len(grid)):
    for j in range(len(grid[0])):
      if grid[i][j] in ["^", ">", "v", "<"]:
        return (i, j)
  assert False
  return (-1, -1)

def canTurn(dirIndex, currPos, grid):
  if dirIndex in [0, 2]:
    if currPos[1] - 1 >= 0:
      if grid[currPos[0]][currPos[1] - 1] == '#':
        return True
    if currPos[1] + 1 < len(grid[0]):
      if grid[currPos[0]][currPos[1] + 1] == '#':
        return True
  elif dirIndex in [1, 3]:
    if currPos[0] - 1 >= 0:
      if grid[currPos[0] - 1][currPos[1]] == '#':
        return True
    if currPos[0] + 1 < len(grid):
      if grid[currPos[0] + 1][currPos[1]] == '#':
        return True
  return False

def inRange(nextPos, grid):
  return nextPos[0] >= 0 and nextPos[0] < len(grid) and nextPos[1] >= 0 and nextPos[1] < len(grid[0])

def getTurn(directions, dirIndex, currPos, grid):
  nextIndex = (dirIndex - 1) % len(directions)
  nextPos = tuple(map(sum, list(zip(currPos, directions[nextIndex]))))
  if inRange(nextPos, grid) and grid[nextPos[0]][nextPos[1]] == '#':
    return (nextIndex, "L")
  return ((dirIndex + 1) % len(directions),  "R")

def part2(codes):
  intcode = Intcode(codes)
  generator = intcode.run()

  grid = []
  row = []
  for value in generator:
    if value != 10:
      row.append(chr(value))
    else:
      grid.append(row)
      row = []
  grid = grid[:-1]

  currPos = getPos(grid)
  moves = []

  directions = [(-1, 0), (0, 1), (1, 0), (0, -1)]
  dirIndex = 0
  while canTurn(dirIndex, currPos, grid):
    dirIndex, turn = getTurn(directions, dirIndex, currPos, grid)
    moves.append(turn)
    steps = 0

    nextPos = tuple(map(sum, list(zip(currPos, directions[dirIndex]))))
    while inRange(nextPos, grid) and grid[nextPos[0]][nextPos[1]] == '#':
      steps += 1
      currPos = nextPos
      nextPos = tuple(map(sum, list(zip(currPos, directions[dirIndex]))))
    moves.append(steps)

  # PATTERNS
  # A: L10, R8, R6, R10
  # B: L12, R8, L12
  # C: L10, R8, R8
  patternA = "L,10,R,8,R,6,R,10\n"
  patternB = "L,12,R,8,L,12\n"
  patternC = "L,10,R,8,R,8\n"
  patternA = [ord(x) for x in patternA]
  patternB = [ord(x) for x in patternB]
  patternC = [ord(x) for x in patternC]

  # STRUCTURE
  # A B A B C C B A C A
  structure = "A,B,A,B,C,C,B,A,C,A\n"
  structure = [ord(x) for x in structure]

  # NO VIDEO FEED
  videoFeed = "n\n"
  videoFeed = [ord(x) for x in videoFeed]

  intcode.reset()
  intcode.replace(0, 2)
  intcode.setInput(structure + patternA + patternB + patternC + videoFeed)
  *_, spaceDust = intcode.run()

  return spaceDust

codes = []
with open("./data/q17.txt", "r") as file:
  line = file.readline()
  codes = [int(x) for x in line.strip().split(",")]

print("Answer to part1: {}".format(part1(codes[:])))
print("Answer to part2: {}".format(part2(codes[:])))