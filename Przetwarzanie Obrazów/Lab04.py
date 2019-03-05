import numpy as np


def zadC1(image):
	return np.histogram(image, bins=256, range=(0, 256), density=True)[0]


def testC1():
	result = zadC1(np.array([[255, 255, 0],
							 [128, 128, 0],
							 [255, 0, 128]]))
	expected = np.array([0.33333333, 0, 0, 0, 0, 0,
 						  0, 0, 0, 0,  0,  0,
 						  0,  0,  0,  0,  0,  0,
 						  0,  0,  0,  0,  0,  0,
						  0,  0,  0,  0,  0,  0,
						  0,  0,  0,  0,  0,  0,
						  0,  0,  0,  0,  0,  0,
						  0,  0,  0,  0,  0,  0,
						  0,  0,  0,  0,  0,  0,
						  0,  0,  0,  0,  0,  0,
						  0,  0,  0,  0,  0,  0,
						  0,  0,  0,  0,  0,  0,
						  0,  0,  0,  0,  0,  0,
						  0,  0,  0,  0,  0,  0,
						  0,  0,  0,  0,  0,  0,
						  0,  0,  0,  0,  0,  0,
						  0,  0,  0,  0,  0,  0,
						  0,  0,  0,  0,  0,  0,
						  0,  0,  0,  0,  0,  0,
						  0,  0,  0,  0,  0,  0,
						  0,  0,  0,  0,  0,  0,
						  0,  0,  0.33333333, 0,  0,  0,
						  0,  0,  0,  0,  0,  0,
						  0,  0,  0,  0,  0,  0,
						  0,  0,  0,  0,  0,  0,
						  0,  0,  0,  0,  0,  0,
						  0,  0,  0,  0,  0,  0,
						  0,  0,  0,  0,  0,  0,
						  0,  0,  0,  0,  0,  0,
						  0,  0,  0,  0,  0,  0,
						  0,  0,  0,  0,  0,  0,
						  0,  0,  0,  0,  0,  0,
						  0,  0,  0,  0,  0,  0,
						  0,  0,  0,  0,  0,  0,
						  0,  0,  0,  0,  0,  0,
						  0,  0,  0,  0,  0,  0,
						  0,  0,  0,  0,  0,  0,
						  0,  0,  0,  0,  0,  0,
						  0,  0,  0,  0,  0,  0,
						  0,  0,  0,  0,  0,  0,
						  0,  0,  0,  0,  0,  0,
						  0,  0,  0,  0,  0,  0,
					 	  0,  0,  0,  0.33333333])

	np.testing.assert_allclose(result, expected)


def zadC2(image):
	img_min = np.min(image)
	img_max = np.max(image)
	return np.rint(((image - img_min)/(img_max-img_min))*255)


def testC2():
	result = zadC2(np.array([[1, 0, 0],
					  		 [0, 4, 1],
					  		 [5, 0, 1]]))
	expected = [[51, 0, 0],
 				[0, 204, 51],
 				[255, 0, 51]]

	
	np.testing.assert_array_equal(result, expected)


def zadC3(image):
	hist = zadC1(image)
	D = np.cumsum(hist)
	D0 = D[D > 0][0]
	return np.rint(255*(D[image] - D0)/(1 - D0))


def testC3():
	result = zadC3(np.array([[1, 0, 0],
							 [0, 255, 1],
							 [5, 0, 1]]))
	expected = [[153, 0, 0],
 				[0, 255, 153],
 				[204, 0, 153]]

	np.testing.assert_array_equal(result, expected)


testC1()
testC2()
testC3()
