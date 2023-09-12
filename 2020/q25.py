def part1(cardPKey, doorPKey):
  cardVal, doorVal = 1, 1
  subjectNumber = 7
  denominator = 20201227
  cardLoopSize, doorLoopSize = 0, 0

  while cardVal != cardPKey:
    cardVal *= subjectNumber
    cardVal = cardVal % denominator
    cardLoopSize += 1

  while doorVal != doorPKey:
    doorVal *= subjectNumber
    doorVal = doorVal % denominator
    doorLoopSize += 1

  cardVal, doorVal = 1, 1

  for _ in range(0, cardLoopSize):
    doorVal *= doorPKey
    doorVal = doorVal % denominator
  
  for _ in range(0, doorLoopSize):
    cardVal *= cardPKey
    cardVal = cardVal % denominator
  
  assert cardVal == doorVal
  return cardVal

with open("./data/q25.txt", "r") as file:
  cardPKey = int(file.readline().strip())
  doorPKey = int(file.readline().strip())

print("Answer to part1: {}".format(part1(cardPKey, doorPKey)))