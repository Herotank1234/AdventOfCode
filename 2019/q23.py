from Intcode import Intcode
from collections import defaultdict

NUM_OF_COMPUTERS = 50

def part1(codes):
  finalAddress = 255
  computers = defaultdict(dict)
  inboxes = defaultdict(list)
  for id in range(NUM_OF_COMPUTERS):
    computers[id]["intcode"] = Intcode(codes[:], input=[id])
    computers[id]["gen"] = computers[id]["intcode"].run()

  while finalAddress not in inboxes:
    for id in range(NUM_OF_COMPUTERS):
      if not inboxes[id]:
        computers[id]["intcode"].addInput(-1)
      else:
        values = inboxes[id].pop(0)
        _ = [computers[id]["intcode"].addInput(value) for value in values]
      address = next(computers[id]["gen"])
      if address is None:
        continue
      x = next(computers[id]["gen"])
      y = next(computers[id]["gen"])
      inboxes[address].append((x, y))
  
  return inboxes[finalAddress][0][1]

def networkIsIdle(inboxes):
  for id in range(NUM_OF_COMPUTERS):
    if inboxes[id]:
      return False
  return True

def part2(codes):
  computers = defaultdict(dict)
  inboxes = defaultdict(list)
  for id in range(NUM_OF_COMPUTERS):
    computers[id]["intcode"] = Intcode(codes[:], input=[id])
    computers[id]["gen"] = computers[id]["intcode"].run()

  NATAddress = 255
  NATLastPacket = None
  NATLastDeliveredPacket = None
  found = False

  while not found:
    for id in range(NUM_OF_COMPUTERS):
      if not inboxes[id]:
        computers[id]["intcode"].addInput(-1)
      else:
        values = inboxes[id].pop(0)
        _ = [computers[id]["intcode"].addInput(value) for value in values]
      address = next(computers[id]["gen"])
      if address is None:
        continue
      else:
        x = next(computers[id]["gen"])
        y = next(computers[id]["gen"])
        if address != NATAddress:
          inboxes[address].append((x, y))
        else:
          NATLastPacket = (x, y)
          if networkIsIdle(inboxes):
            if NATLastDeliveredPacket is not None and NATLastPacket[1] == NATLastDeliveredPacket[1]:
              return NATLastPacket[1]
            else:
              inboxes[0].append(NATLastPacket)
              NATLastDeliveredPacket = NATLastPacket
  
  return -1

codes = []
with open("./data/q23.txt", "r") as file:
  line = file.readline().strip().split(",")
  codes = [int(x) for x in line]

print("Answer to part1: {}".format(part1(codes[:])))
print("Answer to part2: {}".format(part2(codes[:])))