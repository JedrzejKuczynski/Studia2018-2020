import numpy as np
import matplotlib.pyplot as plt

#Zad. 1
def odd_matrix(matrix):
	return matrix[:, 1::2]

#Zad. 2
def reverse_columns(matrix):
	return matrix[:, ::-1]

#Zad. 3
def sum_of_rectangle(matrix, x, y, width, height):
	return np.sum(matrix[y:y+height, x:x+width])

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

#Zad. 7
def expand_matrix(matrix, k):
	if isinstance(k, int) and k >= 0:
		zeros_ver = np.zeros((k, matrix.shape[1]), dtype=np.uint8)
		matrix = np.vstack((matrix, zeros_ver))
		matrix = np.vstack((zeros_ver, matrix))
		zeros_hor = np.zeros((matrix.shape[0], k), dtype=np.uint8)
		matrix = np.hstack((matrix, zeros_hor))
		matrix = np.hstack((zeros_hor, matrix))
		return matrix
	else:
		return -1
		

#Zad. 8
def row_out(x):
	return x[1:, :] - x[:-1, :]

#Zad. 9
def image_RGB(x, y, r, g, b):
	return np.array([[[r, 0, 0]*x, [0, g, 0]*x, [0, 0, b]*x]*y]).reshape(y, x*3, 3)
