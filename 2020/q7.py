import re

class Bag:
  def __init__(self, name, children):
    self.name = name
    self.children = children

def containsGoldBag(bag, bags):
  bagObj = bags.get(bag)
  children = bagObj.children
  for child in children:
    if child[1] == "shiny gold":
      return True
    else:
      if(containsGoldBag(child[1], bags)):
        return True
  return False


def part1(bags):
  bagsThatContainGoldBags = 0
  for bag in bags:
    if containsGoldBag(bag, bags):
      bagsThatContainGoldBags += 1
  return bagsThatContainGoldBags

def sumBags(name, bags):
  bagObj = bags.get(name)
  if not bagObj.children:
    return 1
  else:
    return sum([int(child[0]) * sumBags(child[1], bags) for child in bagObj.children]) + 1

def part2(bags):
  return sumBags("shiny gold", bags) - 1

bags = {}

with open("./data/q7.txt", "r") as file:
  for line in file:
    line = line.split(" bags contain")
    children = re.findall(f"([\d] [a-z]+ [a-z]+|no other) bag[s]?", line[1])
    children = [child.split(" ", 1) if child != "no other" else None for child in children]
    if children == [None]:
      children = []
    bags[line[0]] = Bag(line[0], children)

print("Answer to part1: {}".format(part1(bags)))
print("Answer to part2: {}".format(part2(bags)))