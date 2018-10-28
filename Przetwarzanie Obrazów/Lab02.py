import numpy as np
import matplotlib.pyplot as plt

#Zad. 1
def odd_matrix(matrix):
	return matrix[:, 1::2]

#Zad. 2
def reverse_columns(matrix):
	return matrix[:, ::-1]

#Zad. 3 Później
def sum_of_rectangle(matrix, x, y, width, height):
	return np.sum(matrix[y]) #???? razem z krawędzią, bez?

#Zad. 4
def sum_of_powers(numbers, k):
	return np.sum(numbers**k)

#Zad. 5
def two_matrices(x, y):
	if x.shape != y.shape:
		return -1
	else:
		return x*y

#Zad. 6
def sum_to_x(numbers, x):
	return np.sum(numbers[numbers < x])

#Zad. 7 Później
#def enlarge_matrix(matrix, k):
#	if isinstance(k, int) and k >= 0:

#Zad. 8 Później
def row_out(x):
	pass

#Zad. 9
def image_RGB(x, y, r, g, b):
	img = np.zeros((y, x*3, 3), dtype=np.uint8)
	img[:, :x] = [r, 0, 0]
	img[:, x:2*x] = [0, g, 0]
	img[:, 2*x:] = [0, 0, b]
	return img

image = image_RGB(10, 30, 245, 200, 255)
print(image.shape)

plt.imshow(image)
plt.show()
