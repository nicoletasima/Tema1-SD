import matplotlib.pyplot as plt
import numpy as np
import sys

def read_x_y(path):
    xs = []
    ys = []

    with open(path, "r") as f:
        for line in f.readlines()[1:]:
            tokens = line.split(" ")
            x, y = int(tokens[0]), float(tokens[1])
            xs.append(x)
            ys.append(y)
    return np.array(xs), np.array(ys)

def main():
    x, y = read_x_y(sys.argv[1])
    plt.plot(x, y, 'bo')
    plt.xlabel('timestamp(ms)')
    plt.ylabel('value')
    plt.show()

if __name__ == "__main__":
    main()
