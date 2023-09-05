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
      colour = painting.get((i, j), 0)
      print(("#" if colour else " "), end="")
    print("\n", end="")
  return

def part1(codes):
  directions = [(-1, 0), (0, 1), (1, 0), (0, -1)]
  painting = {}
  currDir = 0
  currPos = (0, 0)
  currCode = codes[:]
  intcode = Intcode(currCode)
  generator = intcode.run()

  while not intcode.isHalted():
    intcode.addInput(painting.get(currPos, 0))
    nextColour = next(generator, None)
    if nextColour is None:
      break
    nextTurn = next(generator, None)
    if nextTurn is None:
      break

    painting[currPos] = nextColour

    currDir = (currDir + (1 if nextTurn else -1)) % len(directions)
    currPos = tuple(map(sum, zip(currPos, directions[currDir])))

  return len(painting)

def part2(codes):
  directions = [(-1, 0), (0, 1), (1, 0), (0, -1)]
  painting = {(0, 0): 1}
  currDir = 0
  currPos = (0, 0)
  currCode = codes[:]
  intcode = Intcode(currCode)
  generator = intcode.run()

  while not intcode.isHalted():
    intcode.addInput(painting.get(currPos, 0))
    nextColour = next(generator, None)
    if nextColour is None:
      break
    nextTurn = next(generator, None)
    if nextTurn is None:
      break

    painting[currPos] = nextColour

    currDir = (currDir + (1 if nextTurn else -1)) % len(directions)
    currPos = tuple(map(sum, zip(currPos, directions[currDir])))

  showImage(painting)
  return "HJALJZFH"

codes = []
with open("./data/q11.txt", "r") as file:
  for line in file:
    codes = [int(x) for x in line.strip().split(",")]
codes += [0 for _ in range(len(codes))]

print("Answer to part1: {}".format(part1(codes)))
print("Answer to part2: {}".format(part2(codes)))