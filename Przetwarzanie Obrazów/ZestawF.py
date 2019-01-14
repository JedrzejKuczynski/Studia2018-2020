import numpy as np

test_area1 = [(0, 0), (1, 0), (1, 1), (0, 1)]
test_area2 = [(1, 1), (2, 1), (3, 1), (4, 1), (4, 2), (4, 3), (4, 4),
		 (4, 5), (3, 5), (2, 5), (2, 4), (2, 3), (1, 3), (1, 2)]
test_edge1 = [(3, 1), (4, 2), (5, 3), (5, 2), (6, 1)]

def zadF1(contour):
	contour_sum = 0
	contour.insert(0, contour[-1])
	for i in range(len(contour) - 1):
		x = contour[i][0]
		y = contour[i][1]
		contour_sum += (y * (contour[i + 1][0] - x) - x * (contour[i + 1][1] - y))
	return abs(0.5 * contour_sum)
	

def testF1():
	result = zadF1(test_area2)
	expected = 10
	np.testing.assert_array_equal(result, expected)

def zadF2(contour):
	contour.append(contour[0])
	contour.insert(0, contour[-2])
	for i in range(1, len(contour) - 1):
		x_prev = contour[i - 1][0]
		y_prev = contour[i - 1][1]
		x_middle = contour[i][0]
		y_middle = contour[i][1]
		x_next = contour[i + 1][0]
		y_next = contour[i + 1][1]
		dx_prev = x_middle - x_prev
		dy_prev = y_middle - y_prev
		
		

testF1()
zadF2(test_edge1)
