def tryMatch(message, index, ruleName, rules):
  if index >= len(message):
    return []
  
  rule = rules[ruleName]
  if rule[0][0][0] == '\"':
    if rule[0][0][1] == message[index]:
      return [index + 1]
    else:
      return []
  
  matchedIndices = []
  for partRule in rule:
    currMatchedIndices = [index]
    for ruleNumber in partRule:
      partMatchedIndices = []
      for currIndex in currMatchedIndices:
        partMatchedIndices += tryMatch(message, currIndex, ruleNumber, rules)
      currMatchedIndices = partMatchedIndices
    matchedIndices += partMatchedIndices
  return matchedIndices


def part1(rules, messages):
  matching = 0
  for message in messages:
    matchedIndices = tryMatch(message, 0, '0', rules)
    if len(message) in matchedIndices:
      matching += 1
  return matching

def part2(rules, messages):
  rules['8'] = [['42'], ['42', '8']]
  rules['11'] = [['42', '31'], ['42', '11', '31']]
  matching = 0
  for message in messages:
    matchedIndices = tryMatch(message, 0, '0', rules)
    if len(message) in matchedIndices:
      matching += 1
  return matching

rules = {}
messages = []

with open("./data/q19.txt", "r") as file:
  goToMessage = False
  for line in file:
    if line == "\n":
      goToMessage = True
    elif not goToMessage:
      line = line.strip().split(": ")
      children = line[1].split(" | ")
      children = [rule.split(" ") for rule in children]
      rules[line[0]] = children
    else:
      messages.append(line.strip())

print("Answer to part1: {}".format(part1(rules, messages)))
print("Answer to part2: {}".format(part2(rules, messages)))