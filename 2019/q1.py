from re import M

def part1(modules):
  total = 0
  for m in modules:
    total += ((m // 3) - 2)
  return total

def part2(modules):
  total = 0
  for m in modules:
    currMass = m
    while currMass >= 0:
      currMass = ((currMass // 3) - 2)
      if currMass > 0:
        total += currMass
  return total

modules = []
with open("./data/q1.txt", "r") as file:
  for line in file:
    modules.append(int(line.strip()))

print("Answer to part1: {}".format(part1(modules)))
print("Answer to part2: {}".format(part2(modules)))