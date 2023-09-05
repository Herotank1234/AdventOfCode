import itertools
import copy
from Intcode import Intcode

NUMBER_OF_AMPLIFIERS = 5

def part1(codes):
  phaseSequences = list(itertools.permutations(list(range(5))))
  amplifiers = [Intcode(copy.deepcopy(codes)) for _ in range(NUMBER_OF_AMPLIFIERS)]
  biggestThrust = 0

  for ps in phaseSequences:
    _ = [amp.reset() for amp in amplifiers]
    inputs = [[i, 0] for i in ps]

    for i in range(len(amplifiers)):
      currAmplifier = amplifiers[i]
      currAmplifier.setInput(inputs[i])
      output = next(currAmplifier.run())

      if i < len(inputs) - 1:
        inputs[i + 1][1] = output

    if output > biggestThrust:
      biggestThrust = output

  return biggestThrust

def part2(codes):
  phaseSequences = list(itertools.permutations(list(range(5, 10))))
  amplifiers = [Intcode(copy.deepcopy(codes)) for _ in range(NUMBER_OF_AMPLIFIERS)]
  initialInput = 0
  biggestThrust = 0

  for ps in phaseSequences:
    _ = [amp.reset() for amp in amplifiers]
    inputs = [[i] for i in ps]
    _ = [amp.setInput(inputs[i]) for i, amp in enumerate(amplifiers)]
    amplifiers[0].addInput(initialInput)
    gens = [amp.run() for amp in amplifiers]
    outputs = [0 for _ in range(NUMBER_OF_AMPLIFIERS)]
      
    while not all([amp.isHalted() for amp in amplifiers]):
      for i in range(NUMBER_OF_AMPLIFIERS):
        output = next(gens[i], None)
        if output is not None:
          amplifiers[(i + 1) % len(amplifiers)].addInput(output)
          outputs[i] = output

    if outputs[-1] > biggestThrust:
      biggestThrust = outputs[-1]

  return biggestThrust

codes = []
with open("./data/q7.txt", "r") as file:
  for line in file:
    codes = [int(x) for x in line.strip().split(",")]

print("Answer to part1: {}".format(part1(codes)))
print("Answer to part2: {}".format(part2(codes)))