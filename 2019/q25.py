from Intcode import Intcode
def part1(instructions):
  intcode = Intcode(instructions)
  gen = intcode.run()
  
  while True:
    output = []
    try:
      word = next(gen)
    except:
      print("".join([chr(x) for x in output]))
      return None

    while word is not None:
      output.append(word)
      try:
        word = next(gen)
      except:
        print("".join([chr(x) for x in output]))
        return None

    print("".join([chr(x) for x in output]))
    command_str = input()
    command_str += '\n'

    command_ascii = [ord(x) for x in command_str]
    for val in command_ascii:
      intcode.addInput(val)

  return None

instructions = []
with open("./data/q25.txt", "r") as file:
  line = file.readline()
  instructions = [int(x) for x in line.split(",")]

print("Answer to part1: {}".format(part1(instructions[:])))