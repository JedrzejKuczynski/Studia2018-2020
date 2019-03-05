import numpy as np

test_area1 = [(0, 0), (1, 0), (1, 1), (0, 1)]
test_area2 = [(1, 1), (2, 1), (3, 1), (4, 1), (4, 2), (4, 3), (4, 4),
		 (4, 5), (3, 5), (2, 5), (2, 4), (2, 3), (1, 3), (1, 2)]
test_edge1 = [(3, 1), (4, 2), (5, 3), (5, 2), (6, 1)]
test_edge2 = [(1, 1), (2, 2), (3, 3), (4, 4), (5, 5), (5, 6),
			  (6, 5), (6, 4), (7, 3)]
contour_kasia = [(1, 1), (4, 1), (4, 5), (2, 5), (2, 3), (1, 1)]

test_image = np.array([[1, 0, 0],
					   [0, 255, 1],
					   [70, 0, 69]])

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
	curvature = []
	for i in range(1, len(contour) - 1):
		x_prev = contour[i - 1][0]
		y_prev = contour[i - 1][1]
		x_middle = contour[i][0]
		y_middle = contour[i][1]
		x_next = contour[i + 1][0]
		y_next = contour[i + 1][1]
		dx_prev = x_middle - x_prev
		dy_prev = y_middle - y_prev
		dx_next = x_next - x_middle
		dy_next = y_next - y_middle
		curvature.append(((dx_next - dx_prev) ** 2) + ((dy_next - dy_prev) ** 2))
	return curvature


def testF2():
	test_copy = test_edge2[:]
	result = zadF2(test_copy)
	expected = [58, 0, 0, 0, 1, 5, 1, 1, 50]
	np.testing.assert_array_equal(result, expected)


def zadF3(contour):
	mean_x = sum([x[0] for x in contour]) / len(contour)
	mean_y = sum([y[1] for y in contour]) / len(contour)
	distances = []
	for x, y in contour:
		dist = ((x - mean_x) ** 2 + (y - mean_y) ** 2) ** 0.5
		distances.append(dist)
	return distances


def testF3():
	result = zadF3(contour_kasia)
	expected = [2.134375, 2.357023, 2.867442, 2.357023, 0.471405, 2.134375]
	np.testing.assert_allclose(result, expected, rtol=1e-5)


def zadF4(image, p, q):
	moment = 0
	for y, x in np.ndindex(image.shape):
		moment += (x**p) * (y**q) * image[y, x]
	return moment


def testF4():
	result = zadF4(test_image, 2, 3)
	expected = 2467
	np.testing.assert_equal(result, expected)


def zadF5(image, p, q):
	mean_x = zadF4(image, 1, 0) / zadF4(image, 0, 0)
	mean_y = zadF4(image, 0, 1) / zadF4(image, 0, 0)
	central_moment = 0
	for y, x in np.ndindex(image.shape):
		central_moment += (x - mean_x)**p * (y - mean_y)**q * image[y, x]
	return central_moment


def testF5():
	result = zadF5(test_image, 2, 3)
	expected = 35.956954412084244
	np.testing.assert_equal(result, expected)
	

testF1()
testF2()
testF3()
testF4()
testF5()
