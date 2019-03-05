from math import sqrt

#Zad. 1
def median(a, b, c):
	x = [a, b, c]
	x.remove((min(x)))
	x.remove(max(x))
	return x[0]

#Zad. 2
def median_sorting(numbers):
	return sorted(numbers)[len(numbers)//2]

#Zad. 3
def sum_of_list(numbers):
	return sum(numbers)

#Zad. 4
def sum_of_numbers(numbers):
	positives = [num for num in numbers if num >= 0]
	negatives = [num for num in numbers if num < 0]
	return sum(positives), sum(negatives)

#Zad. 5
def sum_of_powers(numbers, k):
	return sum([num**k for num in numbers])

#Zad. 6
def distance(v1, v2):
	if len(v1) != len(v2):
		return -1
	else:
		return sqrt(sum([(x - y)**2 for x, y in zip(v1, v2)]))

#Zad. 7
def sum_start_end(numbers, x, r):
	total = 0
	for i in range(x-r, x+r+1):
		if i < 0 or i > len(numbers)-1:
			pass
			#total += 3
		else:
			total += numbers[i]

	return total

#Zad. 8
def sum_of_matrix(x, y):
	matrix = [[x[i]*y[j] for j in range(len(y))] for i in range(len(x))]
	return sum([sum(row) for row in matrix])
