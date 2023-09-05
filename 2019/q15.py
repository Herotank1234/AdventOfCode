from Intcode import Intcode

def showImage(painting):
  minI, maxI, minJ, maxJ = 100_000, -100_000, 100_000, -100_000
  for pos, _ in painting.items():
    if pos[0] < minI:
      minI = pos[0]
    if pos[0] > maxI:
      maxI = pos[0]
    if pos[1] < minJ:
      minJ = pos[1]
    if pos[1] > maxJ:
      maxJ = pos[1]
  
  for i in range(minI, maxI + 1):
    for j in range(minJ, maxJ + 1):
      colour = painting.get((i, j), " ")
      print(colour, end="")
    print("\n", end="")
  return

directions = [(-1, 0), (1, 0), (0, -1), (0, 1)]
returnDirections = {0: 1, 1: 0, 2: 3, 3: 2}

def traverse(intcode, generator, grid, position, returnDirection):
  toTraverse = [x for x in range(4)]
  if returnDirection is not None:
    toTraverse.remove(returnDirection)

  for direction in toTraverse:
    nextPos = tuple(map(sum, zip(position, directions[direction])))
    if nextPos not in grid:
      intcode.addInput(direction + 1)
      status = next(generator)

      if status == 0:
        grid[nextPos] = '#'
      elif status == 1:
        grid[nextPos] = '.'
        traverse(intcode, generator, grid, nextPos, returnDirections[direction])
      elif status == 2:
        grid[nextPos] = 'O'
        traverse(intcode, generator, grid, nextPos, returnDirections[direction])
      else:
        print("Status: " + str(status) + " not recognised")

  if returnDirection is not None:
    intcode.addInput(returnDirection + 1)
    next(generator)

def part1(codes):
  intcode = Intcode(codes)
  generator = intcode.run()
  initialPos = (0, 0)
  grid = {initialPos: "."}
  traverse(intcode, generator, grid, initialPos, None)

  toVisit = [((0, 0), 0)]
  visited = set()
  found = False
  shortestPathLength = -1

  # Breadth first search of the grid
  while not found:
    currPosLength = toVisit.pop(0)
    currPos = currPosLength[0]
    currPathLength = currPosLength[1]

    if currPos in visited:
      continue

    visited.add(currPos)

    if grid[currPos] == "O":
      shortestPathLength = currPathLength
      found = True
      continue

    for direction in directions:
      nextPos = tuple(map(sum, zip(currPos, direction)))
      if nextPos in grid and nextPos not in visited and grid[nextPos] != "#":
        toVisit.append((nextPos, currPathLength + 1))

  assert shortestPathLength != -1
  return shortestPathLength

def part2(codes):
  intcode = Intcode(codes)
  generator = intcode.run()
  initialPos = (0, 0)
  grid = {initialPos: "."}
  traverse(intcode, generator, grid, initialPos, None)

  oxyTankpos = list(grid.keys())[list(grid.values()).index(("O"))]
  toVisit = [oxyTankpos]
  visited = set()
  minutes = 0

  while "." in grid.values():
    nextToVisit = []

    while toVisit:
      currPos = toVisit.pop(0)

      if currPos in visited:
        continue

      visited.add(currPos)

      for direction in directions:
        nextPos = tuple(map(sum, zip(currPos, direction)))
        if nextPos in grid and nextPos not in visited and grid[nextPos] == ".":
          grid[nextPos] = "O"
          nextToVisit.append(nextPos)

    toVisit = nextToVisit
    minutes += 1

  return minutes

codes = []

with open("./data/q15.txt", "r") as file:
  line = file.readline()
  codes = [int(x) for x in line.strip().split(",")]

print("Answer to part1: {}".format(part1(codes[:])))
print("Answer to part2: {}".format(part2(codes[:])))