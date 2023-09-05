from Intcode import Intcode

def part1(codes):
  currCodes = codes[:]
  intcode = Intcode(currCodes, input=[1])
  *testValues, boostCode = intcode.run()
  assert not any(testValues)
  return boostCode

def part2():
  currCodes = codes[:]
  intcode = Intcode(currCodes, input=[2])
  coords = next(intcode.run())
  return coords

codes = []
with open("./data/q9.txt", "r") as file:
  for line in file:
    codes = [int(x) for x in line.strip().split(",")]
codes += [0 for _ in range(len(codes))]

print("Answer to part1: {}".format(part1(codes)))
print("Answer to part2: {}".format(part2()))