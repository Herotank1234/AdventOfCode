from Intcode import Intcode

def part1(codes):
  currCodes = codes[:]
  intcode = Intcode(currCodes, input=[1])
  *testResults, lastVal = intcode.run()
  assert not any(testResults)
  return lastVal

def part2():
  currCodes = codes[:]
  intcode = Intcode(currCodes, input=[5])
  return next(intcode.run())

codes = []
with open("./data/q5.txt", "r") as file:
  for line in file:
    codes = [int(x) for x in line.strip().split(",")]

print("Answer to part1: {}".format(part1(codes)))
print("Answer to part2: {}".format(part2()))