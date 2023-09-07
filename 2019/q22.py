import re
from collections import deque

NUM_OF_CARDS = 10007

def dealIntoNewStack(cards):
  return deque(reversed(cards))

def dealWithIncrement(cards, increment):
  nextCards = deque([-1 for _ in range(len(cards))])
  index = 0
  while cards:
    nextCards[index % len(nextCards)] = cards.popleft()
    index += increment
  return nextCards

def cut(cards, cut):
  cutCards = []
  if cut < 0:
    for _ in range(abs(cut)):
      cutCards.append(cards.pop())
    while cutCards:
      cards.appendleft(cutCards.pop(0))
  else:
    for _ in range(abs(cut)):
      cutCards.append(cards.popleft())
    while cutCards:
      cards.append(cutCards.pop(0))
  return cards

def part1(instructions):
  cards = deque(range(NUM_OF_CARDS))
  for instruction in instructions:
    if instruction == "deal into new stack":
      cards = dealIntoNewStack(cards)
    elif "deal with increment" in instruction:
      increment = int(re.search(r"[\d]+", instruction).group(0))
      cards = dealWithIncrement(cards, increment)
    elif "cut" in instruction:
      cutLength = int(re.search(r"\-?[\d]+", instruction).group(0))
      cards = cut(cards, cutLength)
    else:
      print("Instruction not matched: {}".format(instruction))

  index = 0
  for value in cards:
    if value == 2019:
      break
    index += 1
  return index

# Representing the shuffles as a linear congruential function
# f(x) = ax + b mod m
#   (where m is the length of array)
# Returns (a, b) for each shuffle

# Index x goes to m - x - 1
# f(x) = -x - 1 mod m
def dealNew():
  return (-1, -1)

# Index x goes to x - n
# f(x) = x - n mod m
def cutN(n):
  return (1, -n)

# Index x goes to nx mod m
# f(x) = nx mod m
def dealN(n):
  return (n, 0)

def compose(coeff1, coeff2, length):
  a = (coeff1[0] * coeff2[0]) % length
  b = ((coeff1[1] * coeff2[0]) + coeff2[1]) % length
  return (a, b)

def pow_compose(coeff, length, reps):
  result = (1, 0)
  while reps > 0:
    if reps % 2 != 0:
      result = compose(result, coeff, length)
    reps = reps // 2
    coeff = compose(coeff, coeff, length)
  return result

def modInverse(a, n, m):
  result = 1
  while n > 0:
    if n % 2 != 0:
      result = (result * a) % m
    n = n // 2
    a = (a * a) % m
  return result

def part2(instructions):
  # Compose all the steps into a single linear congruential function
  # f(x) = ax + b mod m
  # g(x) = cx + d mod m
  # g(f(x)) = c(ax + b) + d mod m
  #         = acx + bc + d mod m
  # Hence the coefficents would be (a * c mod m, b * c + d mod m)
  
  coeff = (1, 0)
  length = 119_315_717_514_047
  for instruction in instructions:
    nextCoeff = (1, 1)
    if instruction == "deal into new stack":
      nextCoeff = dealNew()
    elif "deal with increment" in instruction:
      increment = int(re.search(r"[\d]+", instruction).group(0))
      nextCoeff = dealN(increment)
    elif "cut" in instruction:
      cutLength = int(re.search(r"\-?[\d]+", instruction).group(0))
      nextCoeff = cutN(cutLength)

    coeff = compose(coeff, nextCoeff, length)
  
  # Compose by the number of times we need to shuffle
  reps = 101_741_582_076_661
  coeff = pow_compose(coeff, length, reps)

  # Now we have a function to find the end index of a card given the initial index
  # F(x) = Ax + B mod m
  # We want to take the inverse of this function since we are given the end index and
  # want to find the initial index (which is equivalent to the value on the card)
  # x = A * F^-1(x) + B mod m
  # F^-1(x) = (x - B) / A mod m

  # a / b mod m == a * b^-1 mod m
  # a^-1 mod m == a^(m - 2) mod m
  return ((2020 - coeff[1]) * modInverse(coeff[0], length - 2, length)) % length

instructions = []
with open("./data/q22.txt", "r") as file:
  for line in file:
    instructions.append(line.strip())

print("Answer to part1: {}".format(part1(instructions)))
print("Answer to part2: {}".format(part2(instructions)))