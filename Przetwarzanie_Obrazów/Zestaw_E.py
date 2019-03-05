import numpy as np


test_image = np.array([[0, 1, 1, 1],
					   [1, 1, 1, 1],
					   [0, 1, 0, 1],
					   [0, 1, 1, 1]])

test_kernel = np.array([[0, 1, 0],
						[0, 1, 1],
						[0, 0, 0]])


def zadE1(image, kernel):
	k_col = kernel.shape[1] // 2
	k_row = kernel.shape[0] // 2
	padded_image = np.pad(image, ((k_row, k_row), (k_col, k_col)), "constant")
	for y, x in np.ndindex(image.shape):
		result = np.sum(padded_image[y:y+kernel.shape[0], x:x+kernel.shape[1]]*kernel)
		if result > 0:
			image[y, x] = 1
		else:
			image[y, x] = 0
	return image

def testE1():
	result = zadE1(test_image.copy(), test_kernel)
	expected = [[1, 1, 1, 1],
				[1, 1, 1, 1],
				[1, 1, 1, 1],
				[1, 1, 1, 1]]
	np.testing.assert_array_equal(result, expected)


def zadE2(image, kernel):
	k_col = kernel.shape[1] // 2
	k_row = kernel.shape[0] // 2
	padded_image = np.pad(image, ((k_row, k_row), (k_col, k_col)), "constant")
	for y, x in np.ndindex(image.shape):
		result = np.sum(padded_image[y:y+kernel.shape[0], x:x+kernel.shape[1]]*kernel)
		if result == np.sum(kernel):
			image[y, x] = 1
		else:
			image[y, x] = 0
	return image


def testE2():
	result = zadE2(test_image.copy(), test_kernel)
	expected = [[0, 0, 0, 0],
				[0, 1, 1, 0],
				[0, 0, 0, 0],
				[0, 1, 0, 0]]
	np.testing.assert_array_equal(result, expected)

def zadE3(image, kernel):
	kernel_first = kernel[::-1, ::-1]
	image = zadE1(image, kernel_first)
	image = zadE2(image, kernel)
	return image


def testE3():
	result = zadE3(test_image.copy(), test_kernel)
	expected = [[0, 0, 0, 0],
 				[1, 1, 1, 0],
 				[0, 1, 1, 0],
 				[0, 1, 1, 0]]
	np.testing.assert_array_equal(result, expected)


def zadE4(image, kernel):
	kernel_first = kernel[::-1, ::-1]
	image = zadE2(image, kernel_first)
	image = zadE1(image, kernel)
	return image


def testE4():
	result = zadE4(test_image.copy(), test_kernel)
	expected = [[0, 1, 1, 1],
				[1, 1, 1, 1],
				[0, 1, 0, 1],
				[0, 0, 0, 0]]
	np.testing.assert_array_equal(result, expected)


testE1()
testE2()
testE3()
testE4()
