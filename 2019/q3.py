from pickletools import int4
import re

def getCoords(wire):
  x, y = 0, 0
  coords = set()
  for (dir, dist) in wire:
    if dir == "R":
      coords.update([(x + a + 1, y) for a in range(dist)])
      x += dist
    elif dir == "U":
      coords.update([(x, y + a + 1) for a in range(dist)])
      y += dist
    elif dir == "L":
      coords.update([(x - a - 1, y) for a in range(dist)])
      x -= dist
    elif dir == "D":
      coords.update([(x, y - a - 1) for a in range(dist)])
      y -= dist
  return coords

def getSteps(wire, coord):
  targetX, targetY = coord
  wirex, wirey= 0, 0
  steps = 0
  for (dir, dist) in wire:
    matched = False
    if dir == "R":
      for _ in range(dist):
        wirex += 1
        steps += 1
        if wirex == targetX and wirey == targetY:
          matched = True
          break
    elif dir == "U":
      for _ in range(dist):
        wirey += 1
        steps += 1
        if wirex == targetX and wirey == targetY:
          matched = True
          break
    elif dir == "L":
      for _ in range(dist):
        wirex -= 1
        steps += 1
        if wirex == targetX and wirey == targetY:
          matched = True
          break
    elif dir == "D":
      for _ in range(dist):
        wirey -= 1
        steps += 1
        if wirex == targetX and wirey == targetY:
          matched = True
          break

    if matched:
      break
  return steps

def part1(wire1, wire2):
  coords1 = getCoords(wire1)
  coords2 = getCoords(wire2)

  intersect = coords1.intersection(coords2)
  return min(list(map(lambda a: abs(a[0]) + abs(a[1]), intersect)))

def part2():
  coords1 = getCoords(wire1)
  coords2 = getCoords(wire2)

  intersect = coords1.intersection(coords2)
  steps = []
  for inter in intersect:
    steps.append(getSteps(wire1, inter) + getSteps(wire2, inter))
  return min(steps)

wire1 = []
wire2 = []

def splitOp(op):
  direction = op[0]
  distance = int(re.search(r"[\d]+", op).group())
  return(direction, distance)

with open("./data/q3.txt", "r") as file:
  wire1 = list(map(splitOp, file.readline().strip().split(",")))
  wire2 = list(map(splitOp, file.readline().strip().split(",")))

print("Answer to part1: {}".format(part1(wire1, wire2)))
print("Answer to part2: {}".format(part2()))