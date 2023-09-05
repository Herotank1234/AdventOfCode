def adjacent(password):
  strRep = str(password)
  for i in range(len(strRep) - 1):
    if int(strRep[i]) == int(strRep[i + 1]):
      return True
  return False

def adjacentLarger(password):
  strRep = str(password)
  index = 0
  while index < (len(strRep) - 1):
    if index < (len(strRep) - 2):
      if int(strRep[index]) == int(strRep[index + 1]) and \
        int(strRep[index]) != int(strRep[index + 2]):
        return True
      elif int(strRep[index]) == int(strRep[index + 1]) and \
        int(strRep[index]) == int(strRep[index + 2]):
        currVal = strRep[index]
        while index + 1 < (len(strRep) - 1) and \
          strRep[index + 1] == currVal:
          index += 1
    else:   
      if int(strRep[index]) == int(strRep[index + 1]):
        return True
    index += 1
  return False

def neverDecrease(password):
  strRep = str(password)
  for i in range(len(strRep) - 1):
    if int(strRep[i]) > int(strRep[i + 1]):
      return False
  return True

def part1(bound):
  valid = 0
  for i in range(bound[0], bound[1] + 1):
    valid += 1 if adjacent(i) and neverDecrease(i) else 0
  return valid

def part2(bound):
  valid = 0
  for i in range(bound[0], bound[1] + 1):
    valid += 1 if adjacentLarger(i) and neverDecrease(i) else 0
  return valid

bound = []
with open("./data/q4.txt", "r") as file:
  for line in file:
    bound = [int(x) for x in line.strip().split("-")]

print("Answer to part1: {}".format(part1(bound)))
print("Answer to part2: {}".format(part2(bound)))