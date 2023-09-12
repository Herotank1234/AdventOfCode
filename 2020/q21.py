import re

class Recipe:
  def __init__(self, ingredients, allergies):
    self._ingeredients = ingredients
    self._allergies = allergies

  def getAllergies(self):
    return self._allergies
  
  def getIngredients(self):
    return self._ingeredients
  
def getAllergies(recipes):
  allergies = set([allergy for sublist in [recipe.getAllergies() for recipe in recipes] for allergy in sublist])
  allergies = {allergy: None for allergy in allergies}
  for recipe in recipes:
    for allergy in recipe.getAllergies():
      if allergies[allergy] is None:
        allergies[allergy] = set(recipe.getIngredients())
      else:
        allergies[allergy] = allergies[allergy].intersection(set(recipe.getIngredients()))

  changes = -1
  while changes != 0:
    changes = 0
    confirmed = [(k, v) for k, v in allergies.items() if len(v) == 1]
    for allergy, ingredient in confirmed:
      ing = list(ingredient)[0]
      for k, v in allergies.items():
        if k != allergy:
          if ing in v:
            v.remove(ing)
            changes += 1
  return allergies

def part1(recipes):
  allergies = getAllergies(recipes)
  
  ingredients = set([ing for sublist in [recipe.getIngredients() for recipe in recipes] for ing in sublist])
  for _, v in allergies.items():
    if list(v)[0] in ingredients:
      ingredients.remove(list(v)[0])

  noAllergy = 0
  for recipe in recipes:
    for ing in recipe.getIngredients():
      if ing in ingredients:
        noAllergy += 1
  return noAllergy

def part2(recipes):
  allergies = getAllergies(recipes)
  allergies = sorted(allergies.items())
  return ",".join([list(ing)[0] for _, ing in allergies])

recipes = []
with open("./data/q21.txt", "r") as file:
  for line in file:
    line = line.strip().split(" (")
    ingredients = line[0].split(" ")
    allergies = re.search(r"contains ([a-z, ]+)\)", line[1]).group(1).split(", ")
    recipes.append(Recipe(ingredients, allergies))

print("Answer to part1: {}".format(part1(recipes)))
print("Answer to part2: {}".format(part2(recipes)))