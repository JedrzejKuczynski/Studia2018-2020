import numpy as np

def zadA1(image):
	return 255 - image

def testA1():
	result = zadA1(np.array([[0,1,255]], np.uint8))
	expected = [[255,254,0]]
 
	np.testing.assert_array_equal(result, expected)

def zadA2(image, thr):
	image[image > thr] = 255
	image[image <= thr] = 0
	return image

def testA2():
	#normalny
	result = zadA2(np.array([[128, 255, 43, 56],
							[41, 88, 128, 36]], np.uint8), 42)
	expected = [[255, 255, 255, 255],
				[0, 255, 255, 0]]
	np.testing.assert_array_equal(result, expected)
	
	#dla 255 thr=255
	result = zadA2(np.array([[255, 255, 255, 255],
							 [255, 255, 255, 255]], np.uint8), 255)
	expected = [[0, 0, 0, 0],
				[0, 0, 0, 0]]
	np.testing.assert_array_equal(result, expected)

def zadA3(image, gamma):
	image = image.astype(float)
	image /= 255
	image = image**gamma
	return np.rint(image*255)

def testA3():
	#normalny
	result = zadA3(np.array([[255, 128, 43, 0],
							 [128, 87, 20, 255]], np.uint8), 2)
	expected = [[255, 64, 7, 0],
				[64, 30, 2, 255]]
	np.testing.assert_array_equal(result, expected)
	
	#test Kasi
	result = zadA3(np.array([[1, 0, 255],
							 [0, 4, 1],
							 [5, 0, 1]], np.uint8), 0.5)
	expected = [[16, 0, 255],
				[0, 32, 16],
				[36, 0, 16]]
	np.testing.assert_array_equal(result, expected)

def zadA4(image):
	monochromatic = np.zeros((image.shape[0], image.shape[1]))
	monochromatic = 0.21*image[:, :, 0] + 0.72*image[:, :, 1] + 0.07*image[:, :, 2]
	return np.rint(monochromatic)

def testA4():
	#normalny
	result = zadA4(np.array([[[255, 128, 0], [64, 32, 16]],

							 [[128, 0, 255], [255, 255, 0]],

							 [[0, 255, 128], [255, 255, 255]]], np.uint8))
	expected = [[146, 38],
				[45, 237],
				[193, 255]]
	np.testing.assert_array_equal(result, expected)

def zadA5(image):
	image = image.astype(float)
	r, g, b = image[:, :, 0], image[:, :, 1], image[:, :, 2]
	alfa = (2*r - g- b)/2
	beta = (3**(1/2)/2)*(g-b)
	hue = np.arctan2(beta, alfa)
	hue = np.degrees(hue)
	hue %= 360
	return hue

def testA5():
	#normalny
	result = zadA5(np.array([[[255, 128, 0], [64, 32, 16]],

							 [[128, 0, 255], [255, 255, 0]],

							 [[0, 255, 128], [255, 255, 255]]], np.uint8))
	expected = [[30.12972422, 19.10660535],
 		   [270.12972422, 60.        ],
 		   [150.12972422, 0.        ]]
	np.testing.assert_allclose(result, expected)

def zadA6(imageA, imageB, alpha):
	return np.rint(alpha*imageA + (1 - alpha)*imageB)

def testA6():
	#normalny
	result = zadA6(np.array([[255, 0, 128],
							 [32, 16, 128],
							 [255, 255, 255]], np.uint8), np.array([[32, 64, 128],
							 										[255, 255, 255],
							 										[255, 255, 0]], np.uint8), 0.7)
	expected = [[188, 19, 128],
				[99, 88, 166],
				[255, 255, 178]]
	np.testing.assert_array_equal(result, expected)

testA1()
testA2()
testA3()
testA4()
testA5()
testA6()
