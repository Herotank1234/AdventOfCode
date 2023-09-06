from Intcode import Intcode

def part1(codes):
  # CHECK IF THERE IS A HOLE IN THE NEXT 3 SPACES AND THE JUMP DESTINATION IS GROUND
  program = "NOT A J\nNOT B T\nOR T J\nNOT C T\nOR T J\nAND D J\nWALK\n"
  program = [ord(x) for x in program]
  intcode = Intcode(codes, input=program)
  *output, damage = intcode.run()
  for val in output:
    print(chr(val), end="")
  return damage

def part2(codes):
  # STEP 1: CHECK IF CAN WALK AGAIN/JUMP AFTER THE INITIAL JUMP
  # CHECK IF E OR H ARE GROUND -> E OR H === NOT(NOT E AND NOT H)
  # NOT E T
  # NOT H J
  # AND T J
  # NOT J J

  # STEP 2: CHECK IF THE JUMP DESTINATION IS GROUND
  # CHECK IF D IS GROUND
  # AND D J

  # STEP 3: RESET THE T REGISTER
  # MOVE NOT J INTO T, AND J AND T: (J AND NOT J) === FALSE
  # NOT J T
  # AND J T

  # STEP 4: CHECK IF THERE IS A HOLE IN THE NEXT 3 SPACES
  # MOVE A INTO THE RESET T REGISTER, AND B AND C, NOT: NOT(A AND B AND C) === NOT A OR NOT B OR NOT C
  # OR A T
  # AND B T
  # AND C T
  # NOT T T

  # STEP 5: IF THESE IS A HOLE, JUMP
  # AND T J
  
  program = "NOT E T\nNOT H J\nAND T J\nNOT J J\nAND D J\nNOT J T\nAND J T\nOR A T\nAND B T\nAND C T\nNOT T T\nAND T J\nRUN\n"
  program = [ord(x) for x in program]
  intcode = Intcode(codes, input=program)
  *output, damage = intcode.run()
  for val in output:
    print(chr(val), end="")
  return damage

codes = []
with open("./data/q21.txt", "r") as file:
  line = file.readline().strip().split(",")
  codes = [int(x) for x in line]

print("Answer to part1: {}".format(part1(codes[:])))
print("Answer to part2: {}".format(part2(codes[:])))