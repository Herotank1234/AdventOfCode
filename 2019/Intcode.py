INSTRUCTION_LENGTHS = {1: 4, 2: 4, 3: 2, 4: 2, 5: 3, 6: 3, 7: 4, 8: 4, 9: 2, 99: 1}
OPCODE_LENGTHS = {1: 5, 2: 5, 3: 3, 4: 3, 5: 4, 6: 4, 7: 5, 8: 5, 9: 3, 99: 2}

class Intcode:
  def __init__(self, instructions, index=0, input=None, inputIndex=0):
    self._instructions = instructions
    self._originalInstructions = instructions[:]
    self._index = index
    if input is None:
      self._input = []
    else:
      self._input = input
    self._inputIndex = inputIndex
    self._relativeBase = 0
    self._jumped = False
    self._halted = False

    self._extendInitialMemory()

  def _extendInitialMemory(self):
    self._instructions += [0 for _ in range(100_000)]

  def _decodeOpcode(self, opcode):
    completeOpcode = str(opcode)
    opcode = int(completeOpcode[-2:])
    modes = {"modeOpL": 0, "modeOpR": 0, "modeDst": 0}

    while len(completeOpcode) < OPCODE_LENGTHS[opcode]:
      completeOpcode = "0" + completeOpcode

    if OPCODE_LENGTHS[opcode] == 5:
      modes["modeOpL"] = int(completeOpcode[2])
      modes["modeOpR"] = int(completeOpcode[1])
    elif OPCODE_LENGTHS[opcode] == 4:
      modes["modeOpL"] = int(completeOpcode[1])
    
    modes["modeDst"] = int(completeOpcode[0])

    return modes, opcode
  
  def _resolveModes(self, modes, modeLabels):
    resolved = []
    currIndex = self._index + 1
    for label in modeLabels:
      address = self._instructions[currIndex]
      mode = modes[label]
      if mode == 0:
        resolved.append(self._instructions[address])
      elif mode == 1:
        resolved.append(address)
      elif mode == 2:
        resolved.append(self._instructions[self._relativeBase + address])
      currIndex += 1
    return resolved
  
  def _resolveDst(self, modes, index):
    return self._instructions[index] + (self._relativeBase if modes["modeDst"] == 2 else 0)

  def run(self):
    while True:
      self._jumped = False
      modes, opcode = self._decodeOpcode(self._instructions[self._index])

      # ADDITION
      if opcode == 1:
        opL, opR = self._resolveModes(modes, ["modeOpL", "modeOpR"])
        dst = self._resolveDst(modes, self._index + 3)
        self._instructions[dst] = opL + opR

      # MULTIPLICATION
      elif opcode == 2:
        opL, opR = self._resolveModes(modes, ["modeOpL", "modeOpR"])
        dst = self._resolveDst(modes, self._index + 3)
        self._instructions[dst] = opL * opR

      # INPUT
      elif opcode == 3:
        if self._inputIndex == len(self._input):
          yield None
        dst = self._resolveDst(modes, self._index + 1)
        self._instructions[dst] = self._input[self._inputIndex]
        self._inputIndex += 1

      # OUTPUT
      elif opcode == 4:
        dst = self._resolveModes(modes, ["modeDst"])[0]
        yield dst

      # JUMP IF TRUE
      elif opcode == 5:
        op, dst = self._resolveModes(modes, ["modeOpL", "modeDst"])
        if op != 0:
          self._index = dst
          self._jumped = True

      # JUMP IF NOT TRUE
      elif opcode == 6:
        op, dst = self._resolveModes(modes, ["modeOpL", "modeDst"])
        if op == 0:
          self._index = dst
          self._jumped = True

      # LESS THAN
      elif opcode == 7:
        opL, opR = self._resolveModes(modes, ["modeOpL", "modeOpR"])
        dst = self._resolveDst(modes, self._index + 3)
        self._instructions[dst] = int(opL < opR)

      # EQUALS
      elif opcode == 8:
        opL, opR = self._resolveModes(modes, ["modeOpL", "modeOpR"])
        dst = self._resolveDst(modes, self._index + 3)
        self._instructions[dst] = int(opL == opR)

      # MODIFY RELATIVE BASE
      elif opcode == 9:
        dst = self._resolveModes(modes, ["modeDst"])[0]
        self._relativeBase += dst

      # HALT
      elif opcode == 99:
        self._halted = True
        break
      
      # OPCODE NOT RECOGNISED
      else:
        print("No such opcode: " + str(opcode))
      
      self._index += INSTRUCTION_LENGTHS[opcode] if not self._jumped else 0

  def reset(self):
    self._instructions = self._originalInstructions[:]
    self._index = 0
    self._input = []
    self._inputIndex = 0
    self._relativeBase = 0
    self._jumped = False
    self._halted = False
    self._extendInitialMemory()

  def replace(self, index, value):
    self._instructions[index] = value

  def getValue(self, index):
    return self._instructions[index]

  def setInput(self, input):
    self._input = input

  def addInput(self, input):
    self._input.append(input)

  def isHalted(self):
    return self._halted
  
  def getInput(self):
    return self._input
  