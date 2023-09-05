from Intcode import Intcode

ANSWER = 19690720
def part1(codes):
  currCodes = codes[:]
  intcode = Intcode(currCodes)
  intcode.replace(1, 12)
  intcode.replace(2, 2)
  next(intcode.run(), None)
  return intcode.getValue(0)

def part2(codes):
  currCodes = codes[:]
  intcode = Intcode(currCodes)
  for noun in range(100):
    for verb in range(100):
      intcode.reset()
      intcode.replace(1, noun)
      intcode.replace(2, verb)
      next(intcode.run(), None)
      if intcode.getValue(0) == ANSWER:
        return 100 * noun + verb
  return -1

codes = []
with open("./data/q2.txt", "r") as file:
  for line in file:
    codes = [int(x) for x in line.strip().split(",")]

print("Answer to part1: {}".format(part1(codes)))
print("Answer to part2: {}".format(part2(codes)))