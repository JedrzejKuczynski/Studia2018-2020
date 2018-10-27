from math import sqrt

#Zad 1 Trza zrobić

#Zad 2
def median(numbers):
	return sorted(numbers)[len(numbers)//2]

#Zad 3
def sum_of_list(numbers):
	return sum(numbers)

#Zad 4
def sum_of_numbers(numbers):
	positives = [num for num in numbers if num >= 0]
	negatives = [num for num in numbers if num < 0]
	return sum(positives), sum(negatives)

#Zad 5
def sum_of_powers(numbers, k):
	return sum([num**k for num in numbers])

#Zad 6
def distance(v1, v2):
	return sqrt(sum([(x - y)**2 for x, y in zip(v1, v2)]))

#Zad 7 Trza zrobić

#Zad 8 Trza zrobić


