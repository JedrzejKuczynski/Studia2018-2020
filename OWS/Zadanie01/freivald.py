import numpy as np

a = np.loadtxt("liczby.txt")
b = np.loadtxt("liczby.txt")
c = np.loadtxt("wynik.txt")

c_flat = c.flatten()
print(np.max(c_flat))

output = []

for _ in range(100_000):
    r = np.random.randint(2, size=[a.shape[0]])
    br = np.dot(b, r)
    cr = np.dot(c, r)
    abr = np.dot(a, br)
    p = abr - cr
    if not any(p):
        output.append(True)

if all(output):
    print("Mnozenie macierzy poprawne!")
else:
    print("Mnozenie macierzy niepoprawne!")
