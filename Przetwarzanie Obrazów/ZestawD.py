import numpy as np


image = np.array([[1, 2, 3],
				  [0, 5, 0],
				  [3, 1, 3]], dtype="float")

mask = np.array([1, 5, 1], dtype="float")

maskV = np.array([2, 4, 2], dtype="float")

mask2D = np.array([[1, 5, 1],
				 [3, 4, 3],
				 [0, 2, 0]], dtype="float")


def zadD1(image, mask):
	image = image.astype(float)
	length = mask.shape[0]
	padding = length // 2
	padded_image = np.pad(image, ((0, 0), (padding, padding)), "constant")
	for y, x in np.ndindex(image.shape):
		image[y, x] = np.sum(padded_image[y, x:x+length]*mask)
	return image


def testD1():
	result = zadD1(image.copy(), mask)
	expected = [[7, 14, 17],
				[5, 25, 5],
				[16, 11, 16]]
	np.testing.assert_array_equal(result, expected)


def zadD2(image, mask):
	image = image.astype(float)
	length = mask.shape[0]
	padding = length // 2
	padded_image = np.pad(image, ((padding, padding), (0, 0)), "constant")
	for y, x in np.ndindex(image.shape):
		image[y, x] = np.sum(padded_image[y:y+length, x]*mask)
	return image


def testD2():
	result = zadD2(image.copy(), mask)
	expected = [[5, 15, 15],
				[4, 28, 6],
				[15, 10, 15]]
	np.testing.assert_array_equal(result, expected)


def zadD3(image, mask2D):
	image = image.astype(float)
	padding_rows = mask2D.shape[0] // 2
	padding_cols = mask2D.shape[1] // 2
	padded_image = np.pad(image, ((padding_rows, padding_rows), (padding_cols, padding_cols)), "constant")
	for y, x in np.ndindex(image.shape):
		image[y, x] = np.sum(padded_image[y:y+mask2D.shape[0], x:x+mask2D.shape[1]]*mask2D)
	return image


def testD3():
	result = zadD3(image.copy(), mask2D)
	expected = [[10, 30, 18],
				[28, 36, 38],
				[20, 47, 20]]
	np.testing.assert_array_equal(result, expected)


def zadD4(image, maskH, maskV):
	image = image.astype(float)
	image = zadD1(image, maskH)
	image = zadD2(image, maskV)
	mask2D = np.zeros((maskV.shape[0], maskH.shape[0]))
	for y, x in np.ndindex(mask2D.shape):
		mask2D[y, x] = maskV[y] * maskH[x] 
	return image, mask2D


def testD4():
	result_image, result_mask = zadD4(image.copy(), mask, maskV)
	expected_image = [[38, 106, 78],
					  [66, 150, 86],
					  [74, 94, 74]]
	np.testing.assert_array_equal(result_image, expected_image)

	expected_mask = [[2, 10, 2],
					 [4, 20, 4],
					 [2, 10, 2]]
	np.testing.assert_array_equal(result_mask, expected_mask)

testD1()
testD2()
testD3()
testD4()