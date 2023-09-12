from collections import deque
import copy
from itertools import islice

def simulateGame(player1, player2):
  while player1 and player2:
    card1 = player1.popleft()
    card2 = player2.popleft()
    if card1 > card2:
      player1.append(card1)
      player1.append(card2)
    else:
      player2.append(card2)
      player2.append(card1)
  return player1, player2

def getConfig(player1, player2):
  player1str, player2str = "", ""
  for card in player1:
    player1str += str(card)
  
  for card in player2:
    player2str += str(card)

  return (player1str, player2str)

def simulateRecursiveGame(player1, player2, previousConfigs):
  while player1 and player2:
    config = getConfig(player1, player2)
    if config in previousConfigs:
      return "player1", player1, player2
    previousConfigs.add(config)
    card1 = player1.popleft()
    card2 = player2.popleft()
    if card1 <= len(player1) and card2 <= len(player2):
      subPlayer1 = deque(islice(copy.deepcopy(player1), 0, card1))
      subPlayer2 = deque(islice(copy.deepcopy(player2), 0, card2))
      winner, _, _ = simulateRecursiveGame(subPlayer1, subPlayer2, set())
      if winner == "player1":
        player1.append(card1)
        player1.append(card2)
      else:
        player2.append(card2)
        player2.append(card1)
    else:
      if card1 > card2:
        player1.append(card1)
        player1.append(card2)
      else:
        player2.append(card2)
        player2.append(card1)
  if player1:
    return "player1", player1, player2
  else:
    return "player2", player1, player2

def evalScore(player):
  score = 0
  weight = len(player)
  for card in player:
    score += (weight * card)
    weight -= 1
  return score

def part1(player1, player2):
  currPlayer1, currPlayer2 = copy.deepcopy(player1), copy.deepcopy(player2)
  currPlayer1, currPlayer2 = simulateGame(currPlayer1, currPlayer2)
  score = evalScore(currPlayer1) if currPlayer1 else evalScore(currPlayer2)
  return score

def part2(player1, player2):
  currPlayer1, currPlayer2 = copy.deepcopy(player1), copy.deepcopy(player2)
  winner, currPlayer1, currPlayer2 = simulateRecursiveGame(currPlayer1, currPlayer2, set())
  score = evalScore(currPlayer1) if winner == "player1" else evalScore(currPlayer2)
  return score

player1 = deque()
player2 = deque()
with open("./data/q22.txt", "r") as file:
  stage = 0
  for line in file:
    if line == "\n":
      stage += 1
    elif "Player" in line:
      continue
    else:
      if stage == 0:
        player1.append(int(line.strip()))
      else:
        player2.append(int(line.strip()))

print("Answer to part1: {}".format(part1(player1, player2)))
print("Answer to part2: {}".format(part2(player1, player2)))