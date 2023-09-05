import re
import math
import functools

STEPS = 1000

class Moon:
  def __init__(self, x, y, z):
    self.x = x
    self.y = y
    self.z = z
    self.velx = 0
    self.vely = 0
    self.velz = 0

  def applyGravity(self, otherMoon):
    if self.x < otherMoon.x:
      self.velx += 1
    elif self.x > otherMoon.x:
      self.velx -= 1

    if self.y < otherMoon.y:
      self.vely += 1
    elif self.y > otherMoon.y:
      self.vely -= 1

    if self.z < otherMoon.z:
      self.velz += 1
    elif self.z > otherMoon.z:
      self.velz -= 1
  
  def applyGravityOne(self, axis, otherMoon):
    if axis == "x":
      if self.x < otherMoon.x:
        self.velx += 1
      elif self.x > otherMoon.x:
        self.velx -= 1
    elif axis == "y":
      if self.y < otherMoon.y:
        self.vely += 1
      elif self.y > otherMoon.y:
        self.vely -= 1
    elif axis == "z":
      if self.z < otherMoon.z:
        self.velz += 1
      elif self.z > otherMoon.z:
        self.velz -= 1

  def applyVelocity(self):
    self.x += self.velx
    self.y += self.vely
    self.z += self.velz

  def applyVelocityOne(self, axis):
    if axis == "x":
      self.x += self.velx
    elif axis == "y":
      self.y += self.vely
    elif axis == "z":
      self.z += self.velz

  def getHashString(self, axis):
    if axis == "x":
      return str(self.x) + "_" + str(self.velx)
    elif axis == "y":
      return str(self.y) + "_" + str(self.vely)
    elif axis == "z":
      return str(self.z) + "_" + str(self.velz)

  def getEnergy(self):
    return (abs(self.x) + abs(self.y) + abs(self.z)) * \
      (abs(self.velx) + abs(self.vely) + abs(self.velz))

  def prettyPrint(self):
    print("pos=<x=" + str(self.x) + ", y=" + str(self.y) + ", z=" + str(self.z) + ">, vel=<x=" + str(self.velx) + ", y=" + str(self.vely) + ", z=" + str(self.velz) + ">")

def part1(moons):
  for i in range(STEPS):
    #Apply Gravity
    for moon1 in moons:
      for moon2 in moons:
        if moon1 is not moon2:
          moon1.applyGravity(moon2)
    
    #Apply Velocity
    for moon in moons:
      moon.applyVelocity()

  totalEnergy = 0
  for moon in moons:
    totalEnergy += moon.getEnergy()
  return totalEnergy

def lcm(a, b):
  return (a * b) // math.gcd(a, b)

def part2():
  allSteps = []
  for axis in ["x", "y", "z"]:
    hashes = set()
    hashString = moons[0].getHashString(axis) + "_" + \
        moons[1].getHashString(axis) + "_" + \
        moons[2].getHashString(axis) + "_" + \
        moons[3].getHashString(axis)
    stepsTaken = 0
    while hashString not in hashes:
      hashes.add(hashString)
      #Apply Gravity
      for moon1 in moons:
        for moon2 in moons:
          if moon1 is not moon2:
            moon1.applyGravityOne(axis, moon2)
      
      #Apply Velocity
      for moon in moons:
        moon.applyVelocityOne(axis)
      stepsTaken += 1
      hashString = moons[0].getHashString(axis) + "_" + \
        moons[1].getHashString(axis) + "_" + \
        moons[2].getHashString(axis) + "_" + \
        moons[3].getHashString(axis)
    allSteps.append(stepsTaken)

  return functools.reduce(lcm, allSteps)

moons = []
with open("./data/q12.txt", "r") as file:
  for line in file:
    match = re.findall(r"[-]?[\d]+", line)
    moons.append(Moon(int(match[0]), int(match[1]), int(match[2])))

print("Answer to part1: {}".format(part1(moons)))
print("Answer to part2: {}".format(part2()))