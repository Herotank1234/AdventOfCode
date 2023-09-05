import math
import numpy as np

def getAsteroidList(asteroids):
  asteroidCoords = []
  length = len(asteroids)
  width = len(asteroids[0])
  for y in range(length):
    for x in range(width):
      if asteroids[y][x] == "#":
        asteroidCoords.append((x, y))
  return asteroidCoords

def getNumberOfSpottedAsteroids(x, y, asteroidList):
  angles = set()
  spotted = 0
  for (x2, y2) in asteroidList:
    if not (x == x2) or not (y == y2):
      angle = math.atan2(x2 - x, y - y2) * 180 / math.pi
      if angle < 0:
        angle += 360
      if angle not in angles:
        angles.add(angle)
        spotted += 1
  return spotted

def getAsteroidsPerAngle(x, y, asteroidCoords):
  angleMap = {}
  for (x2, y2) in asteroidCoords:
    angle = math.atan2(x2 - x, y - y2) * 180 / math.pi
    if angle < 0:
      angle += 360
    if angle not in angleMap:
      angleMap[angle] = [(x2, y2)]
    else:
      angleMap[angle].append((x2, y2))
  return angleMap

def part1(asteroids):
  asteroidCoords = getAsteroidList(asteroids)
  spotted = []
  for (x, y) in asteroidCoords:
    spotted.append(getNumberOfSpottedAsteroids(x, y, asteroidCoords))
  return max(spotted)

def part2(asteroids):
  asteroidCoords = getAsteroidList(asteroids)
  spotted = []
  for (x, y) in asteroidCoords:
    spotted.append(getNumberOfSpottedAsteroids(x, y, asteroidCoords))
  maxSpottedIndex = np.argmax(spotted)

  maxSpottedCoords = asteroidCoords[maxSpottedIndex]
  asteroidCoords.remove(maxSpottedCoords)

  angleAsteroidMap = getAsteroidsPerAngle(*maxSpottedCoords, asteroidCoords)
  angleAsteroidMap = dict(sorted(angleAsteroidMap.items()))

  for key in angleAsteroidMap:
    angleAsteroidMap[key].sort(key=lambda x: math.sqrt(
      (x[0] - maxSpottedCoords[0]) ** 2 + (x[1] - maxSpottedCoords[1]) ** 2))

  vaporised = 0
  while True:
    for key in angleAsteroidMap:
      if angleAsteroidMap[key]:
        res = angleAsteroidMap[key].pop()
        vaporised += 1
        if vaporised == 200:
          return res[0] * 100 + res[1]


asteroids = []
with open("./data/q10.txt", "r") as file:
  for line in file:
    asteroids.append([*line.strip()])

print("Answer to part1: {}".format(part1(asteroids)))
print("Answer to part2: {}".format(part2(asteroids)))