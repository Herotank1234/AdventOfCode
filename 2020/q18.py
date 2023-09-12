PRECEDENCE = {"(": 2, "*": 1, "+": 1}
ADVANCEDPRECEDENCE = {"(": 2, "*": 0, "+": 1}

def shuntYard(expr, advanced=False):
  prec = PRECEDENCE if not advanced else ADVANCEDPRECEDENCE
  output = []
  operatorStack = []
  for op in expr:
    if isinstance(op, int):
      output.append(op)
    elif op == "(":
      operatorStack.append(op)
    elif op == ")":
      while operatorStack[-1] != "(":
        output.append(operatorStack.pop())
      operatorStack.pop()
    elif op == "*" or op == "+":
      while len(operatorStack) > 0 and \
        prec[operatorStack[-1]] >= prec[op] and \
        operatorStack[-1] != "(":
        output.append(operatorStack.pop())
      operatorStack.append(op)
  while operatorStack:
    output.append(operatorStack.pop())
  return output

def eval(infix):
  res = []
  for op in infix:
    if isinstance(op, int):
      res.append(op)
    elif op == "+":
      op1 = res.pop()
      op2 = res.pop()
      res.append(op1 + op2)
    elif op == "*":
      op1 = res.pop()
      op2 = res.pop()
      res.append(op1 * op2)
  return res[0]

def part1(exprs):
  total = 0
  for expr in exprs:
    infix = shuntYard(expr)
    value = eval(infix)
    total += value
  return total

def part2(exprs):
  total = 0
  for expr in exprs:
    infix = shuntYard(expr, advanced=True)
    value = eval(infix)
    total += value
  return total

def parse(string):
  index = 0
  length = len(string)
  res = []
  while index < length:
    if string[index] in ["(", ")", "+", "*"]:
      res.append(string[index])
    elif string[index].isdigit():
      res.append(int(string[index]))
    index += 1
  return res

exprs = []
with open("./data/q18.txt", "r") as file:
  for line in file:
    exprs.append(parse(line.strip()))

print("Answer to part1: {}".format(part1(exprs)))
print("Answer to part2: {}".format(part2(exprs)))