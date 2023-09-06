from Intcode import Intcode

def testInput(intcode, input):
  intcode.reset()
  intcode.setInput(input)
  return next(intcode.run())

def part1(codes):
  intcode = Intcode(codes)
  inputs = [[i, j] for i in range(0, 50) for j in range(0, 50)]
  inTractorBeam = 0
  
  for input in inputs:
    inTractorBeam += testInput(intcode, input)

  return inTractorBeam

def part2(codes):
  intcode = Intcode(codes)
  i, j = 0, 0
  indent = 0
  found = False
  while not found:
    while testInput(intcode, [i, indent]) == 0:
      indent += 1
    j = indent
    while True:
      if testInput(intcode, [i, j + 99]) == 0:
        i += 1
        break
      else:
        if testInput(intcode, [i + 99, j]) == 0:
          j += 1
          continue
        else:
          found = True
          break

  return i * 10_000 + j

codes = []
with open("./data/q19.txt", "r") as file:
  line = file.readline().strip().split(",")
  codes = [int(x) for x in line]


print("Answer to part1: {}".format(part1(codes[:])))
print("Answer to part2: {}".format(part2(codes[:])))