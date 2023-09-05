import math
from collections import defaultdict

def getQuantity(recipes, name, quantity, inventory):
  inInven = inventory.get(name, 0)
  if inInven >= quantity:
    inventory[name] -= quantity
    return 0
  
  if name == "ORE":
    return quantity
  
  currRecipe = recipes[name]
  ingredients = currRecipe[0]
  outputQuantity = currRecipe[1]

  batches = math.ceil(quantity / outputQuantity)

  oreUsed = 0
  for ingredient in ingredients:
    inInven = inventory[ingredient[0]]
    inventory[ingredient[0]] = 0
    oreUsed += getQuantity(recipes, ingredient[0], ingredient[1] * batches - inInven, inventory)
  
  rem = (batches * outputQuantity) - quantity
  inventory[name] += rem

  return oreUsed

def part1(recipes):
  return getQuantity(recipes, "FUEL", 1, defaultdict(int))

def part2():
  quantity = 3550000
  oreUsed = getQuantity(recipes, "FUEL", quantity, defaultdict(int)) 
  while oreUsed <= 1_000_000_000_000:
    quantity += 1
    oreUsed = getQuantity(recipes, "FUEL", quantity, defaultdict(int)) 
  return quantity - 1

recipes = {}
with open("./data/q14.txt", "r") as file:
  for line in file:
    recipe = line.strip().split(" => ")
    recipe[1] = recipe[1].split(" ")
    recipe[1] = (recipe[1][1], int(recipe[1][0]))
    recipe[0] = recipe[0].split(", ")
    recipe[0] = list(map(lambda x: x.split(" "), recipe[0]))
    recipe[0] = list(map(lambda x: (x[1], int(x[0])), recipe[0]))
    recipes[recipe[1][0]] = (recipe[0], recipe[1][1])

print("Answer to part1: {}".format(part1(recipes)))
print("Answer to part2: {}".format(part2()))