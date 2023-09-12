import re
import copy
import pprint

pp = pprint.PrettyPrinter()

def part1(bounds, nearbyTickets):
  errorRate = 0
  for ticket in nearbyTickets:
    for value in ticket:
      found = False
      for key in bounds:
        for bound in bounds[key]:
          if bound[0] <= value <= bound[1]:
            found = True
            break
        if found:
          break
      if not found:
        errorRate += value
  return errorRate

def reduced(bounds):
  for key in bounds:
    if len(bounds[key]) != 1:
      return False
  return True

def part2(bounds, myTicket, nearbyTickets):
  validTickets = copy.deepcopy(nearbyTickets)
  for ticket in nearbyTickets:
    valid = True
    for value in ticket:
      found = False
      for key in bounds:
        for bound in bounds[key]:
          if bound[0] <= value <= bound[1]:
            found = True
            break
        if found:
          break
      if not found:
        valid = False
    if not valid:
      validTickets.remove(ticket)
  
  boundForCol = {}
  for i in range(len(validTickets[0])):
    validBounds = []
    for key in bounds:
      validForBound = True
      for ticket in validTickets:
        if not (bounds[key][0][0] <= ticket[i] <= bounds[key][0][1] or \
          bounds[key][1][0] <= ticket[i] <= bounds[key][1][1]):
          validForBound = False
          break
      if validForBound:
        validBounds.append(key)
    boundForCol[i] = validBounds

  for i in range(20):
    for key in boundForCol:
      if len(boundForCol[key]) == 1:
        reduceOn = boundForCol[key][0]
        for key1 in boundForCol:
          if (key != key1) and (reduceOn in boundForCol[key1]):
            boundForCol[key1].remove(reduceOn)
  indexes = []
  for key in boundForCol:
    if "departure" in boundForCol[key][0]:
      indexes.append(key)
  
  total = 1
  for index in indexes:
    total *= myTicket[index]
  return total

bounds = {}
myTicket = []
nearbyTickets = []
with open("./data/q16.txt", "r") as file:
  stage = 0
  for line in file:
    if line == "\n":
      stage += 1
    else:
      if stage == 0:
        match = re.search(r".+\:", line.strip())
        bound = re.findall(r"[\d]+\-[\d]+", line.strip())
        bounds[match.group()] = [list(map(int, i)) for i in [b.split("-") for b in bound]]
      elif stage == 1:
        if "your ticket" in line:
          continue
        else:
          a = line.strip().split(",")
          myTicket = [int(x) for x in line.strip().split(",")]
      else:
        if "nearby ticket" in line:
          continue
        else:
          nearbyTickets.append([int(x) for x in line.strip().split(",")])

print("Answer to part1: {}".format(part1(bounds, nearbyTickets)))
print("Answer to part2: {}".format(part2(bounds, myTicket, nearbyTickets)))