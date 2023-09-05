import itertools

PHASES = 100
PATTERN = [0, 1, 0, -1]

def elementGen(digitPosition, pattern):
  patternIndex = 0
  position = 0
  while True:
    yield pattern[patternIndex]
    position += 1
    if position == digitPosition:
      position = 0
      patternIndex = (patternIndex + 1) % len(pattern)

def part1(signal):
  currSignal = signal
  for _ in range(PHASES):
    nextSignal = []
    for i in range(len(currSignal)):
      gen = elementGen(i + 1, PATTERN)
      next(gen)
      coefficients = [next(gen) for _ in range(len(currSignal))]
      multiplied = list(map(lambda a: int(a[0]) * a[1], list(zip(currSignal, coefficients))))
      total = sum(multiplied)
      nextSignal.append(str(total)[-1])

    currSignal = nextSignal

  return "".join(currSignal[:8])

def reverseCumulativeSum(currSignal):
  nextSignal = []
  for i in range(0, len(currSignal)):
    if i == 0:
      nextSignal.append(str(currSignal[len(currSignal) - 1 - i]))
    else:
      nextSignal.append(str((int(currSignal[len(currSignal) - 1 - i]) + int(nextSignal[i - 1])) % 10))
  return nextSignal[::-1]

def part2(signal):
  currSignal = list(itertools.chain.from_iterable([signal for _ in range(10000)]))
  skip = int("".join(currSignal[:7]))
  currSignal = currSignal[skip:]
  for p in range(PHASES):
    nextSignal = reverseCumulativeSum(currSignal)
    currSignal = nextSignal
  return "".join(currSignal[:8])

signal = []
with open("./data/q16.txt", "r") as file:
  line = file.readline()
  signal = [x for x in line.strip()]

print("Answer to part1: {}".format(part1(signal[:])))
print("Answer to part2: {}".format(part2(signal)))