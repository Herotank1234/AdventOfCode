import numpy as np

LENGTH = 6
WIDTH = 25

def splitIntoLayers(image):
  layers = []
  size = LENGTH * WIDTH
  for i in range(len(image) // size):
    layers.append(image[size * i : size * (i + 1)])
  return layers

def part1(image):
  layers = np.array(splitIntoLayers(image))
  zeroCounts = np.count_nonzero(layers == 0, axis=1)
  minArg = np.argmin(zeroCounts)
  return np.count_nonzero(layers[minArg] == 1) * np.count_nonzero(layers[minArg] == 2)

def part2(image):
  layers = np.array(splitIntoLayers(image))
  finalImageIndex = np.argmax(layers <= 1, axis=0)
  finalImage = np.zeros(LENGTH * WIDTH)
  for i in range(len(layers[0])):
    finalImage[i] = layers[finalImageIndex[i]][i]
  # print(finalImage.reshape(LENGTH, WIDTH))
  return "AURCY"

# [[. 0 0 . . 0 . . 0 . 0 0 0 . . . 0 0 . . 0 . . . 0]
#  [0 . . 0 . 0 . . 0 . 0 . . 0 . 0 . . 0 . 0 . . . 0]
#  [0 . . 0 . 0 . . 0 . 0 . . 0 . 0 . . . . . 0 . 0 .]
#  [0 0 0 0 . 0 . . 0 . 0 0 0 . . 0 . . . . . . 0 . .]
#  [0 . . 0 . 0 . . 0 . 0 . 0 . . 0 . . 0 . . . 0 . .]
#  [0 . . 0 . . 0 0 . . 0 . . 0 . . 0 0 . . . . 0 . .]]
# AURCY

image = []
with open("./data/q8.txt", "r") as file:
  for line in file:
    image = [int(x) for x in [*line.strip()]]

print("Answer to part1: {}".format(part1(image)))
print("Answer to part2: {}".format(part2(image)))