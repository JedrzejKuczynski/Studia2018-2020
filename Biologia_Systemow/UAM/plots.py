import matplotlib.pyplot as plt
import numpy as np
import os
import pandas as pd

class1_files = []
class2_files = []

for root, dirs, files in os.walk("."):
    for file in files:
        if "Class1" in file:
            class1_files.append(file)
        elif "Class2" in file:
            class2_files.append(file)

class1_files = sorted(class1_files)
class2_files = sorted(class2_files)

class1_x = []
class1_y = []

for filename in class1_files:
    df = pd.read_csv(filename, sep="\t")
    spikes = df[df["Values[v]"] == 30]
    input_value = spikes["Values[i]"].values[0]
    times = spikes["# Time"].values
    first = times[1:]
    second = times[:-1]
    frequencies = first - second
    frequencies = 1000 / frequencies
    class1_x.append(input_value)
    class1_y.append(frequencies.mean())

class1_x = np.array(class1_x)
class1_y = np.array(class1_y)


plt.plot(class1_x, class1_y.round(2))
plt.savefig("Class1.png")
plt.close()
plt.show()

class2_x = []
class2_y = []

for filename in class2_files:
    df = pd.read_csv(filename, sep="\t")
    spikes = df[df["Values[v]"] == 30]
    input_value = spikes["Values[i]"].values[0]
    times = spikes["# Time"].values
    first = times[1:]
    second = times[:-1]
    frequencies = first - second
    frequencies = 1000 / frequencies
    class2_x.append(input_value)
    class2_y.append(frequencies.mean())

class2_x = np.array(class2_x)
class2_y = np.array(class2_y)


plt.plot(class2_x, class2_y.round(2))
plt.savefig("Class2.png")
plt.close()
plt.show()
